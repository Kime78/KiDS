#pragma once
#include <cstdint>

uint32_t rightRotate(uint32_t number, uint8_t ammount, uint8_t bits);
uint32_t get_opcode(uint32_t opcode);
bool get_bit(uint32_t num, uint8_t bit);
void change_bit(uint32_t &value, bool bit_bal, uint8_t bit_nr);