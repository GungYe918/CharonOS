#pragma once

#include <stdint.h>

extern "C" {
    void WriteIo32(uint16_t addr, uint32_t data);
    uint32_t ReadIo32(uint16_t addr);
}