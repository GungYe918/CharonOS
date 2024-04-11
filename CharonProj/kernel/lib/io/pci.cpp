/**
 * @file pci.cpp
*/

//include - local
#include "pci.hpp"
#include "io_func.h"

//include - system


uint32_t MakePciAddress(
    uint8_t bus, uint8_t device, 
    uint8_t function, uint8_t reg_addr
) {
    return (1u << 31)           // 1을 31자리 왼쪽으로 이동 -> 첫 번째 비트를 1로 설정
        | (bus << 16)            // bus 값을 16비트 왼쪽으로 이동
        | (device << 11)         // device 값을 11비트 왼쪽으로 이동
        | (function << 8)        // function 값을 8비트 왼쪽으로 시프트
        | (reg_addr & 0xfcu);    // reg_addr의 하위 3비트를 사용하여 PCI 주소의 마지막 3비트를 설정
}


namespace {
    using namespace pci;

    //Add device
    Error AddDevice(const Device& device) {
        if (num_device == devices.size()) {
            return MAKE_ERROR(Error::kFull);
        }

        devices[num_device] = device;
        ++num_device;
        return MAKE_ERROR(Error::kSuccess);
    }

    Error ScanBus(uint8_t bus);


    //Scan function
    Error ScanFunction(uint8_t bus, uint8_t device, uint8_t function) {
        auto class_code = ReadClassCode(bus, device, function);
        auto header_type = ReadHeaderType(bus, device, function);
        Device dev{bus, device, function, header_type, class_code};
        if (auto err = AddDevice(dev)) {
            return err;
        }

        if (class_code.Match(0x06u, 0x04u)) {
            //standard pci bridge
            auto bus_numbers = ReadBusNumbers(bus, device, function);
            uint8_t secondary_bus = (bus_numbers >> 8) & 0xffu;
            return ScanBus(secondary_bus);
        }

        return MAKE_ERROR(Error::kSuccess);
    }

    //Scan Device
    Error ScanDevice(uint8_t bus, uint8_t device) {
        if (auto err = ScanFunction(bus, device, 0)) {
            return err;
        }
        if (IsSingleFunctionDevice(ReadHeaderType(bus, device, 0))) {
            return MAKE_ERROR(Error::kSuccess);
        }

        for (uint8_t function = 1; function < 8; ++function) {
            if (ReadVendorId(bus, device, function) == 0xffffu) {
                continue;
            }
            if (auto err = ScanFunction(bus, device, function)) {
                return err;
            }
        }
        return MAKE_ERROR(Error::kSuccess);
    }

    

    //Scan bus
    Error ScanBus(uint8_t bus) {
        for (uint8_t device = 0; device < 32; ++device) {
            if (ReadVendorId(bus, device, 0) == 0xffffu) {
                continue;
            }
            if (auto err = ScanDevice(bus, device)) {
                return err;
            }
        }
        return MAKE_ERROR(Error::kSuccess);
    }
    
}




namespace pci {
    //config addr data
    void WriteAddress(uint32_t address) {
        WriteIo32(kConfigAddress, address);
    }

    void WriteData(uint32_t value) {
        WriteIo32(kConfigData, value);
    }

    uint32_t ReadData() {
        return ReadIo32(kConfigData);
    }

    uint16_t ReadVendorId(
        uint8_t bus, uint8_t device,
        uint8_t function
    ) {
        WriteAddress(MakePciAddress(bus, device, function, 0x00));
        return ReadData() & 0xffffu;
    }

    uint16_t ReadDeviceId(
        uint8_t bus, uint8_t device,
        uint8_t function
    ) {
        WriteAddress(MakePciAddress(bus, device, function, 0x00));
        return ReadData() >> 16; 
    }

    uint8_t ReadHeaderType(
        uint8_t bus, uint8_t device,
        uint8_t function
    ) {
        WriteAddress(MakePciAddress(bus, device, function, 0x0c));
        return (ReadData() >> 16) & 0xffu;
    }

    ClassCode ReadClassCode(
        uint8_t bus, uint8_t device,
        uint8_t function
    ) {
        WriteAddress(MakePciAddress(bus, device, function, 0x08));
        auto reg = ReadData();
        ClassCode cc;
        cc.base         = (reg >> 24) & 0xffu;
        cc.sub          = (reg >> 16) & 0xffu;
        cc.interface    = (reg >>  8) & 0xffu;
        return cc;
    }

    uint32_t ReadBusNumbers(
        uint8_t bus, uint8_t device,
        uint8_t function
    ) {
        WriteAddress(MakePciAddress(bus, device, function, 0x18));
        return ReadData();
    }

    bool IsSingleFunctionDevice(uint8_t header_type) {
        return (header_type & 0x80u) == 0;
    }


    //scan_all_bus
    Error ScanAllBus() {
        num_device = 0;

        auto header_type = ReadHeaderType(0, 0, 0);
        if (IsSingleFunctionDevice(header_type)) {
            return ScanBus(0);
        }

        for (uint8_t function = 0; function < 8; ++function) {
            if (ReadVendorId(0, 0, function) == 0xffffu) {
                continue;
            }
            if (auto err = ScanBus(function)) {
                return err;
            }
        }
        return MAKE_ERROR(Error::kSuccess);
    }

    uint32_t ReadConfigReg(const Device& dev, uint8_t reg_addr) {
        WriteAddress(MakePciAddress(dev.bus, dev.device, dev.function, reg_addr));
        return ReadData();
    }

    void WriteConfigReg(const Device& dev, uint8_t reg_addr, uint32_t value) {
        WriteAddress(MakePciAddress(dev.bus, dev.device, dev.function, reg_addr));
        WriteData(value);
    }

    ValueWithError<uint64_t> ReadBar(Device& device, unsigned int bar_index) {
        if (bar_index >= 6) {
            return {0, MAKE_ERROR(Error::kIndexOutOfRange)};
        }

        const auto addr = CalcBarAddress(bar_index);
        const auto bar = ReadConfigReg(device, addr);

        //32bit address
        if ((bar & 4u) == 0) {
            return {bar, MAKE_ERROR(Error::kSuccess)};
        }

        //64bit address
        if ((bar_index >= 5)) {
            return {0, MAKE_ERROR(Error::kIndexOutOfRange)};
        }

        const auto bar_upper = ReadConfigReg(device, addr+4);

        return {
            bar | (static_cast<uint64_t>(bar_upper) << 32),
            MAKE_ERROR(Error::kSuccess)
        };
    }
    //END
}

