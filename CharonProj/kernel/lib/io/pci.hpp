#pragma once 


//include - system
#include <cstdint>
#include <array>

//include - local
#include "../error/error.hpp"


namespace pci {
    //ConfigAddress - CONFIG_ADDRESS 레지스터의 IO포트 주소
    const uint16_t kConfigAddress = 0x0cf8;

    //ConfigData - CONFIG_DATA 레지스터의 IO포트 주소
    const uint16_t kConfigData = 0x0cfc;


    //define class_code
    struct ClassCode {
        uint8_t base, sub, interface;

        bool Match(uint8_t b) {
            return b == base;
        }

        bool Match(uint8_t b, uint8_t s) {
            return Match(b) && s == sub;
        }

        bool Match(uint8_t b, uint8_t s, uint8_t i) {
            return Match(b, s) && i == interface;
        }
    };



    //define device
    struct Device {
        uint8_t bus, device, function, header_type;
        ClassCode class_code;
    };



    //define pci_func
    void WriteAddress(uint32_t address);
    void WriteData(uint32_t value);
    uint32_t ReadData();

    //define id_read func
    uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function);
    uint16_t ReadDeviceId(uint8_t bus, uint8_t device, uint8_t function);
    uint8_t ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function);
    ClassCode ReadClassCode(uint8_t bus, uint8_t device, uint8_t function);

    inline uint16_t ReadVendorId(const Device& dev) {
        return ReadVendorId(dev.bus, dev.device, dev.function);
    }

    uint32_t ReadConfigReg(const Device& dev, uint8_t reg_addr);

    void WriteConfigReg(const Device& dev, uint8_t reg_addr, uint32_t value);

    
    uint32_t ReadBusNumbers(uint8_t bus, uint8_t device, uint8_t function);



    //check is single function device
    bool IsSingleFunctionDevice(uint8_t header_type);


    inline std::array<Device, 32> devices;
    inline int num_device;


    Error ScanAllBus();

    constexpr uint8_t CalcBarAddress(unsigned int bar_index) {
        return 0x10 + 4 * bar_index;
    }

    ValueWithError<uint64_t> ReadBar(Device& device, unsigned int bar_index);
}