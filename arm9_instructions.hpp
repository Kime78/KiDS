#pragma once

#include <cstdint>

extern void undefined_instruction(uint32_t opcode);
extern void undefined_instruction_thumb(uint16_t opcode);
extern void mov_imm(uint32_t opcode);