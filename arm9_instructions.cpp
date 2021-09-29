#include <iostream>

#include "arm9.hpp"
#include "arm9_instructions.hpp"
#include "math.hpp"

uint32_t get_opcode(uint32_t opcode)
{
    return (((opcode >> 20) & 0xFF) << 4) | ((opcode >> 4) & 0xF);
}

void undefined_instruction(ARM9 *cpu, uint32_t opcode)
{
    std::cout << "[ERROR] UNDEFINED INSTRUCTION " << std::hex << opcode << std::endl;
    exit(0);
}

void undefined_instruction_thumb(ARM9 *cpu, uint16_t opcode)
{
    std::cout << "[ERROR] UNDEFINED INSTRUCTION " << std::hex << opcode << std::endl;
    exit(0);
}

void mov_imm(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t imm = opcode & 0xFF;
    uint8_t shift_ammount = (opcode >> 8) & 0xF;
    cpu->reg[dest] = rightRotate(imm, shift_ammount * 2, 32);

    std::cout << std::hex << "mov imm - reg[" << (int)dest << "] = " << rightRotate(imm, shift_ammount * 2, 32) << '\n';
}

void add_imm(ARM9 *cpu, uint32_t opcode)
{   
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t op1 = (opcode >> 16) & 0xF; 

    uint8_t imm = opcode & 0xFF;
    uint8_t shift_ammount = (opcode >> 8) & 0xF;

    cpu->reg[dest] = cpu->reg[op1] + rightRotate(imm, shift_ammount * 2, 32);

    std::cout << std::hex << "add imm - reg[" << (int)dest << "] = reg[" << (int)op1 << "] + " << rightRotate(imm, shift_ammount * 2, 32) << '\n';
}

void strh(ARM9 *cpu, uint32_t opcode)
{
    std::cout << "strh time";
    
}