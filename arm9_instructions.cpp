#include <iostream>

#include "arm9_instructions.hpp"

void undefined_instruction(uint32_t opcode)
{
    std::cout << "[ERROR] UNDEFINED INSTRUCTION " << std::hex << opcode << std::endl;
    exit(0);
}

void undefined_instruction_thumb(uint16_t opcode)
{
    std::cout << "[ERROR] UNDEFINED INSTRUCTION " << std::hex << opcode << std::endl;
    exit(0);
}

void mov_imm(uint32_t opcode)
{
    std::cout << "mov UwU\n";
    exit(0);
}