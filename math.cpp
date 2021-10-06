#include "math.hpp"

uint32_t rightRotate(uint32_t number, uint8_t ammount, uint8_t bits)
{
    return (number >> ammount) | (number << (bits - ammount));
}

uint32_t get_opcode(uint32_t opcode)
{
    return (((opcode >> 20) & 0xFF) << 4) | ((opcode >> 4) & 0xF);
}

bool get_bit(uint32_t num, uint8_t bit)
{
    return (num >> bit) & 1;
}

void change_bit(uint32_t &value, bool bit_val, uint8_t bit_nr)
{
    int mask = 1 << bit_nr;
    value = ((value & ~mask) | (bit_val << bit_nr));
}

