// Bit manipulation: 16-bit sensor register
// Build: g++ -std=c++17 -Wall -Wextra bits.cpp -o prog && ./prog
//
// A pressure sensor exposes a 16-bit STATUS register:
//   bit 15    : READY   (1 = measurement available)
//   bit 14    : ERROR   (1 = sensor fault)
//   bits 12-13: MODE    (0=standby, 1=active, 2=calibration, 3=reserved)
//   bits 0-11 : VALUE   (raw pressure, unsigned, 0..4095)

#include <cstdint>
#include <iostream>

int getReady(uint16_t reg) { return (reg >> 15) & 0x1; }
int getError(uint16_t reg) { return (reg >> 14) & 0x1; }
int getMode(uint16_t reg)  { return (reg >> 12) & 0x3; }   // 2 bits -> mask 0b11
int getValue(uint16_t reg) { return reg & 0x0FFF; }        // low 12 bits

// Clear: AND with the inverted mask. Set: OR with the mask
uint16_t clearReady(uint16_t reg) {
    return static_cast<uint16_t>(reg & ~(1u << 15));
}
uint16_t setError(uint16_t reg) {
    return static_cast<uint16_t>(reg | (1u << 14));
}

// Two bytes received on the bus (b0 first), little-endian convention:
// b0 = least significant byte
uint16_t fromBytesLE(uint8_t b0, uint8_t b1) {
    return static_cast<uint16_t>(b0 | (static_cast<uint16_t>(b1) << 8));
}

int main() {
    // READY=1, ERROR=0, MODE=1, VALUE=1234
    uint16_t reg = (1u << 15) | (0u << 14) | (1u << 12) | 1234u;

    std::cout << "ready=" << getReady(reg) << " error=" << getError(reg)
              << " mode=" << getMode(reg) << " value=" << getValue(reg) << "\n";

    std::cout << "after clear READY : ready=" << getReady(clearReady(reg)) << "\n";
    std::cout << "after set ERROR   : error=" << getError(setError(reg)) << "\n";

    uint16_t word = fromBytesLE(0xAB, 0xCD);
    std::cout << "frame: AB CD -> little-endian value = 0x" << std::hex
              << std::uppercase << word << std::dec << " (" << word << ")\n";
    return 0;
}

// Notes:
// 1. uint16_t rather than int: guaranteed 16-bit width, int is
//    platform-dependent
// 2. (1u << 15) rather than (1 << 15): on a signed 16-bit int the shift
//    would overflow into the sign bit (UB); 1u = unsigned, safe
// 3. If this register were memory-mapped on real hardware, the pointer
//    would need `volatile`: the value changes outside the program flow,
//    the optimizer must neither cache nor remove the accesses
