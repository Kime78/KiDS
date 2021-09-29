#pragma once
#include <cstdint>

uint32_t rightRotate(uint32_t number, uint8_t ammount, uint8_t bits)
{
    return (number >> ammount) | (number << (bits - ammount));
}