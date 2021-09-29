#pragma once

#include <cstdint>

extern void undefined_instruction(ARM9 *cpu, uint32_t opcode);
extern void undefined_instruction_thumb(ARM9 *cpu, uint16_t opcode);
extern void mov_imm(ARM9 *cpu, uint32_t opcode);
extern void add_imm(ARM9 *cpu, uint32_t opcode);
extern void strh(ARM9 *cpu, uint32_t opcode);