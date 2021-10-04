#pragma once

#include <cstdint>
//math stuff

//ARM9
extern void undefined_instruction(ARM9 *cpu, uint32_t opcode);
extern void undefined_instruction_thumb(ARM9 *cpu, uint16_t opcode);
extern void mov_imm(ARM9 *cpu, uint32_t opcode);
extern void add_imm(ARM9 *cpu, uint32_t opcode);
extern void strh_imm(ARM9 *cpu, uint32_t opcode);
extern void msr_cpsr_reg(ARM9 *cpu, uint32_t opcode);
extern void msr_cpsr_imm(ARM9 *cpu, uint32_t opcode);
extern void msr_spsr_reg(ARM9 *cpu, uint32_t opcode);
extern void msr_spsr_imm(ARM9 *cpu, uint32_t opcode);
extern void strb_imm(ARM9 *cpu, uint32_t opcode);
extern void strb_reg(ARM9 *cpu, uint32_t opcode);
extern void str_imm(ARM9 *cpu, uint32_t opcode);
extern void str_reg(ARM9 *cpu, uint32_t opcode);
extern void ldrb_imm(ARM9 *cpu, uint32_t opcode);
extern void ldrb_reg(ARM9 *cpu, uint32_t opcode);
extern void ldr_imm(ARM9 *cpu, uint32_t opcode);
extern void ldr_reg(ARM9 *cpu, uint32_t opcode);