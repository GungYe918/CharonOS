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

    union CapabilityHeader {
        uint32_t data;

        struct {

            uint32_t cap_id : 8;
            uint32_t next_ptr : 8;
            uint32_t cap : 16;

        } __attribute__((packed)) bits;
    } __attribute__((packed));

    const uint8_t kCapabilityMSI    = 0x05;
    const uint8_t kCapabilityMSIX   = 0x11;

    /**
     * @brief 지정된 PCI 디바이스의 지정된 capability 레지스터를 읽는다.
     * 
     * @param dev - capability를 읽을 PCI 디바이스
     * @param addr - capability 레지스터의 configuration 공간 주소
    */

   CapabilityHeader ReadCapabilityHeader(const Device& dev, uint8_t addr);

   struct MSICapability {
    union {
        uint32_t data;

        struct {

            uint32_t cap_id : 8;
            uint32_t next_ptr : 8;
            uint32_t msi_enable : 1;
            uint32_t multi_msg_capable : 3;
            uint32_t multi_msg_enable : 3;
            uint32_t addr_64_capable : 1;
            uint32_t per_vector_mask_capable : 1;
            uint32_t : 7;

        } __attribute__((packed)) bits;
    } __attribute__((packed)) header;

    uint32_t msg_addr;
    uint32_t msg_upper_addr;
    uint32_t msg_data;
    uint32_t mask_bits;
    uint32_t pending_bits;

   } __attribute__((packed));


    Error ConfigureMSI(const Device& dev, uint32_t msg_addr, 
                        uint32_t msg_data, unsigned int num_vector_exponent);
    /**
        * @brief MSI 또는 MSI-X 인터럽트를 설정하는 코드이다.
        * 
        * @param dev 설정 대상의 PCI 디바이스
        * @param msg_addr 인터럽트 발생 시 메세지를 작성할 주소
        * @param msg_data 인터럽트 발생 시 작성할 메세지의 값
        * @param num_vector_exponent 할당할 벡터 수(2^n에서 n을 지정)
    */

   enum class MSITriggerMode {
        kEdge   = 0,
        kLevel  = 1,
   };

    enum class MSIDeliveryMode {

        kFixed              = 0b000,
        kLowestPriority     = 0b001,
        kSMI                = 0b010,
        kNMI                = 0b100,
        kINIT               = 0b101,
        kExtINT             = 0b111,

    };


    Error ConfigureMSIFixedDestination(
        const Device& dev, uint8_t apic_id,
        MSITriggerMode trigger_mode, MSIDeliveryMode delivery_mode,
        uint8_t vector, unsigned int num_vector_exponent
    );





}