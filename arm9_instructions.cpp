#include <iostream>

#include "arm9.hpp"
#include "arm9_instructions.hpp"
#include "math.hpp"

void undefined_instruction(ARM9 *cpu, uint32_t opcode)
{
    std::cout << "[ERROR] UNDEFINED INSTRUCTION " << std::hex << opcode << ": " << get_opcode(opcode) << std::endl;
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

void strh_imm(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t base = (opcode >> 16) & 0xF;
    bool pre_post = get_bit(opcode, 24);
    bool up_down = get_bit(opcode, 23);
    uint8_t imm = opcode & 0xF;
    bool write_back = 1;
    if (pre_post)
        write_back = get_bit(opcode, 21);

    uint32_t address;
    if (pre_post)
        address = up_down ? cpu->reg[base] + imm : cpu->reg[base] - imm;
    else
        address = cpu->reg[base];

    cpu->reg[dest] = cpu->mem.read16(address);

    if (pre_post == 0)
        address = up_down ? cpu->reg[base] + imm : cpu->reg[base] - imm;

    if (write_back)
        cpu->reg[base] = address;

    std::cout << std::hex << "strh imm reg[" << dest << "] = [" << address << "](" << cpu->mem.read16(address) << ")\n";
}

void msr_cpsr_reg(ARM9 *cpu, uint32_t opcode)
{
    bool flags = get_bit(opcode, 19);
    bool status = get_bit(opcode, 18);
    bool extension = get_bit(opcode, 17);
    bool control = get_bit(opcode, 16);

    uint8_t source = opcode & 0xF;

    if (flags)
        cpu->cpsr = cpu->cpsr & (((cpu->reg[source] & 0xFF) << 24) | 0xFFFFFF);

    //TODO: STATUS & CONTROL???

    if (control)
        cpu->cpsr = cpu->cpsr & ((0xFFFFFF << 8) | cpu->reg[source] & 0xFF);

    std::cout << "msr cpsr reg[" << (int)source << "]\n";
}

void msr_cpsr_imm(ARM9 *cpu, uint32_t opcode)
{
    std::cout << "msr cpsr imm not implemented";
    exit(0);
}

void msr_spsr_reg(ARM9 *cpu, uint32_t opcode)
{
    std::cout << "msr spsr reg not implemented";
    exit(0);
}

void msr_spsr_imm(ARM9 *cpu, uint32_t opcode)
{
    std::cout << "msr spsr imm not implemented";
    exit(0);
}

void strb_imm(ARM9 *cpu, uint32_t opcode)
{
    std::cout << "strb imm not implemented";
    exit(0);
}
void strb_reg(ARM9 *cpu, uint32_t opcode)
{
    std::cout << "strb reg not implemented";
    exit(0);
}
void str_imm(ARM9 *cpu, uint32_t opcode) 
{
    std::cout << "str imm not implemented";
    exit(0);
}
void str_reg(ARM9 *cpu, uint32_t opcode) 
{
    std::cout << "str reg not implemented";
    exit(0);
}
void ldrb_imm(ARM9 *cpu, uint32_t opcode) 
{
    std::cout << "ldrb imm not implemented";
    exit(0);
}
void ldrb_reg(ARM9 *cpu, uint32_t opcode) 
{
    std::cout << "ldrb reg not implemented";
    exit(0);
}
void ldr_imm(ARM9 *cpu, uint32_t opcode) 
{
    std::cout << "ldr imm not implemented";
    exit(0);
}
void ldr_reg(ARM9 *cpu, uint32_t opcode) 
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t base = (opcode >> 16) & 0xF;
    bool pre_post = get_bit(opcode, 24);
    bool up_down = get_bit(opcode, 23);

    bool write_back = 1;
    if (pre_post)
        write_back = get_bit(opcode, 21);

    uint32_t val;
    uint8_t offset = cpu->reg[opcode & 0xF];
    uint16_t shift_amount = (opcode >> 7) & 0b111'1111;
    uint8_t shift_type = (opcode >> 5) & 0b11;

    switch (shift_type)
    {
    case 0b00:
        val = offset << shift_amount;
        break;
    case 0b01:
        val = offset >> shift_amount;
        break;
    case 0b10:
        val = offset >> shift_amount;//gcc only?
        break;
    case 0b11:
        val = rightRotate(offset, shift_amount, 32);
        break;        
    default:
        break;
    }

    uint32_t address;
    if (pre_post)
        address = up_down ? cpu->reg[base] + val : cpu->reg[base] - val;
    else
        address = cpu->reg[base];

    cpu->mem.write32(address, cpu->reg[dest]);

    if (pre_post == 0)
        address = up_down ? cpu->reg[base] + val : cpu->reg[base] - val;

    if (write_back)
        cpu->reg[base] = address;

    std::cout << std::hex << "ldr imm [" << address << "] = reg[" << (int)dest << "]\n";
}