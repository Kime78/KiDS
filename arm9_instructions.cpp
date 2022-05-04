#include <iostream>

#include "arm9.hpp"
#include "arm9_instructions.hpp"
#include "math.hpp"

uint32_t shift(uint8_t shift_type, uint32_t value, uint8_t shift_amount, bool &carry)
{
    uint32_t ret;
    carry = 0;
    switch (shift_type)
    {
    case 0b00:
        carry = (value << (shift_amount - 1)) & 1;
        ret = value << shift_amount;
        break;
    case 0b01:
        carry = (value >> (shift_amount - 1)) & 1;
        ret = value >> shift_amount;
        break;
    case 0b10:
        carry = (value >> (shift_amount - 1)) & 1;
        ret = value >> shift_amount; //gcc only?
        break;
    case 0b11:
        carry = (value >> (shift_amount - 1)) & 1;
        ret = rightRotate(value, shift_amount, 32);
        break;
    default:
        break;
    }
    return ret;
}
void undefined_instruction(ARM9 *cpu, uint32_t opcode)
{
    std::cout << cpu->reg[15] <<  "[ERROR] UNDEFINED INSTRUCTION " << std::hex << opcode << ": " << get_opcode(opcode) << std::endl;
    exit(0);
}

void undefined_instruction_thumb(ARM9 *cpu, uint16_t opcode)
{
    std::cout << cpu->reg[15] <<  "[ERROR] UNDEFINED INSTRUCTION " << std::hex << opcode << std::endl;
    exit(0);
}

void mov_imm(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t imm = opcode & 0xFF;
    uint8_t shift_amount = (opcode >> 8) & 0xF;
    cpu->reg[dest] = rightRotate(imm, shift_amount * 2, 32);

    std::cout << cpu->reg[15] <<  std::hex << " mov imm - reg[" << (int)dest << "] = " << rightRotate(imm, shift_amount * 2, 32) << '\n';
}

void add_imm(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t op1 = (opcode >> 16) & 0xF;

    uint8_t imm = opcode & 0xFF;
    uint8_t shift_amount = (opcode >> 8) & 0xF;

    cpu->reg[dest] = cpu->reg[op1] + rightRotate(imm, shift_amount * 2, 32);

    std::cout << cpu->reg[15] <<  std::hex << " add imm - reg[" << (int)dest << "] = reg[" << (int)op1 << "] + " << rightRotate(imm, shift_amount * 2, 32) << '\n';
}

void ands_imm(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t op1 = (opcode >> 16) & 0xF;

    uint8_t imm = opcode & 0xFF;
    uint8_t shift_amount = (opcode >> 8) & 0xF;

    cpu->reg[dest] = cpu->reg[op1] & rightRotate(imm, shift_amount * 2, 32);

    bool zero = cpu->reg[dest] == 0;
    bool sign = cpu->reg[dest] >> 31;

    change_bit(cpu->cpsr, zero, 30);
    change_bit(cpu->cpsr, sign, 31);

    std::cout << cpu->reg[15] <<  std::hex << " ands imm - reg[" << (int)dest << "] = reg[" << (int)op1 << "] & " << rightRotate(imm, shift_amount * 2, 32) <<  "(" << cpu->reg[dest] << ")" << '\n';
}
void ldrh_imm(ARM9 *cpu, uint32_t opcode) {
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

    std::cout << cpu->reg[15] <<  std::hex << " ldrh imm reg[" << int(dest) << "] = [" << address << "](" << cpu->mem.read16(address) << ")\n";

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

    cpu->mem.write16(address, cpu->reg[dest] & 0xFFFF);

    if (pre_post == 0)
        address = up_down ? cpu->reg[base] + imm : cpu->reg[base] - imm;

    if (write_back)
        cpu->reg[base] = address;

    std::cout << cpu->reg[15] <<  std::hex << " strh imm reg[" << dest << "] = [" << address << "](" << cpu->mem.read16(address) << ")\n";
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

    std::cout << cpu->reg[15] <<  " msr cpsr reg[" << (int)source << "]\n";
}

void msr_cpsr_imm(ARM9 *cpu, uint32_t opcode)
{
    std::cout << cpu->reg[15] <<  " msr cpsr imm not implemented";
    exit(0);
}

void msr_spsr_reg(ARM9 *cpu, uint32_t opcode)
{
    std::cout << cpu->reg[15] <<  " msr spsr reg not implemented";
    exit(0);
}

void msr_spsr_imm(ARM9 *cpu, uint32_t opcode)
{
    std::cout << cpu->reg[15] <<  " msr spsr imm not implemented";
    exit(0);
}

void strb_imm(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t base = (opcode >> 16) & 0xF;
    bool pre_post = get_bit(opcode, 24);
    bool up_down = get_bit(opcode, 23);

    bool write_back = 1;
    if (pre_post)
        write_back = get_bit(opcode, 21);

    uint32_t val = opcode & 0b1111'1111'1111;
    uint32_t address;

    if (pre_post)
        address = up_down ? cpu->reg[base] + val : cpu->reg[base] - val;
    else
        address = cpu->reg[base];

    if(base == 15)
        address += 8;

    cpu->mem.write8(address, cpu->reg[dest] & 0xFF);

    if (pre_post == 0)
        address = up_down ? cpu->reg[base] + val : cpu->reg[base] - val;

    if (write_back)
        cpu->reg[base] = address;

    std::cout << cpu->reg[15] <<  std::hex << " str imm [" << address << "] = reg[" << (int)dest << "]\n"; 
}
void strb_reg(ARM9 *cpu, uint32_t opcode)
{
    std::cout << cpu->reg[15] <<  " strb reg not implemented";
    exit(0);
}
void str_imm(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t base = (opcode >> 16) & 0xF;
    bool pre_post = get_bit(opcode, 24);
    bool up_down = get_bit(opcode, 23);

    bool write_back = 1;
    if (pre_post)
        write_back = get_bit(opcode, 21);

    uint32_t val = opcode & 0b1111'1111'1111;
    uint32_t address;

    if (pre_post)
        address = up_down ? cpu->reg[base] + val : cpu->reg[base] - val;
    else
        address = cpu->reg[base];

    if(base == 15)
        address += 8;

    cpu->mem.write32(address, cpu->reg[dest]);

    if (pre_post == 0)
        address = up_down ? cpu->reg[base] + val : cpu->reg[base] - val;

    if (write_back)
        cpu->reg[base] = address;

    std::cout << cpu->reg[15] <<  std::hex << " str imm [" << address << "] = reg[" << (int)dest << "]\n";
}
void str_reg(ARM9 *cpu, uint32_t opcode)
{
    std::cout << cpu->reg[15] <<  " str reg not implemented";
    exit(0);
}
void ldrb_imm(ARM9 *cpu, uint32_t opcode)
{
    std::cout << cpu->reg[15] <<  " ldrb imm not implemented";
    exit(0);
}
void ldrb_reg(ARM9 *cpu, uint32_t opcode)
{
    std::cout << cpu->reg[15] <<  " ldrb reg not implemented";
    exit(0);
}
void ldr_imm(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t base = (opcode >> 16) & 0xF;
    bool pre_post = get_bit(opcode, 24);
    bool up_down = get_bit(opcode, 23);

    bool write_back = 1;
    if (pre_post)
        write_back = get_bit(opcode, 21);

    uint32_t val = opcode & 0b1111'1111'1111;
    uint32_t address;

    if (pre_post)
        address = up_down ? cpu->reg[base] + val : cpu->reg[base] - val;
    else
        address = cpu->reg[base];

    if(base == 15)
        address += 8;

    cpu->reg[dest] = cpu->mem.read32(address);

    if (pre_post == 0)
        address = up_down ? cpu->reg[base] + val : cpu->reg[base] - val;

    if (write_back)
        cpu->reg[base] = address;

    std::cout << cpu->reg[15] <<  std::hex << " ldr imm [" << address << "] = reg[" << (int)dest << "]\n";
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

    bool a;
    val = shift(shift_type, offset, shift_amount, a);

    uint32_t address;
    if (pre_post)
        address = up_down ? cpu->reg[base] + val : cpu->reg[base] - val;
    else
        address = cpu->reg[base];

    cpu->reg[dest] = cpu->mem.read32(address);

    if (pre_post == 0)
        address = up_down ? cpu->reg[base] + val : cpu->reg[base] - val;

    if (write_back)
        cpu->reg[base] = address;

    std::cout << cpu->reg[15] <<  std::hex << " ldr reg [" << address << "] = reg[" << (int)dest << "]\n";
}

void sub_reg(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t op1 = (opcode >> 16) & 0xF;
    uint8_t op2 = opcode & 0xF;

    uint32_t val;
    bool shift_by_reg = get_bit(opcode, 4);
    uint8_t shift_type = (opcode >> 5) & 0b11;
    if (shift_by_reg)
    {
        uint8_t rs = cpu->reg[(opcode >> 8) & 0xF] & 0xF;
        bool a;
        val = shift(shift_type, cpu->reg[op2], rs, a);
    }
    else
    {
        uint8_t imm = (opcode >> 7) & 0b1'1111;
        bool a;
        val = shift(shift_type, cpu->reg[op2], imm, a);
    }

    cpu->reg[dest] = cpu->reg[op1] - val;

    std::cout << cpu->reg[15] <<  std::hex << " sub reg - reg[" << (int)dest << "] = reg[" << (int)op1 << "] - " << val << '\n';
}

void mov_reg(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t op1 = (opcode >> 16) & 0xF;
    uint8_t op2 = opcode & 0xF;

    uint32_t val;
    bool shift_by_reg = get_bit(opcode, 4);
    uint8_t shift_type = (opcode >> 5) & 0b11;
    if (shift_by_reg)
    {
        uint8_t rs = cpu->reg[(opcode >> 8) & 0xF] & 0xF;
        bool a;
        val = shift(shift_type, cpu->reg[op2], rs, a);
    }
    else
    {
        uint8_t imm = (opcode >> 7) & 0b1'1111;
        bool a;
        val = shift(shift_type, cpu->reg[op2], imm, a);
    }

    cpu->reg[dest] = cpu->reg[op2];

    std::cout << cpu->reg[15] <<  std::hex << " mov reg - reg[" << (int)dest << "] = reg[" << (int)op1 << "] - " << val << '\n';
}

void b(ARM9 *cpu, uint32_t opcode)
{
    int32_t addr = opcode & 0xFFFFFF;
    //BUG SIGN EXTEND ADDR TO 32BIT
    addr <<= 2;
    if(get_bit(addr, 23))
        addr |= 0xFF000000;
    cpu->reg[15] += addr + 4;
    std::cout << cpu->reg[15] <<  std::hex << " b " << addr << '\n';
}
void bl(ARM9 *cpu, uint32_t opcode)
{
    int32_t addr = opcode & 0xFFFFFF;
    cpu->reg[14] = cpu->reg[15];
    addr = addr << 2;
    if(get_bit(addr, 23))
        addr |= 0xFF000000;
    cpu->reg[15] += addr + 4;
    std::cout << cpu->reg[15] <<  std::hex << " bl " << addr << '\n';
}

void add_reg(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t op1 = (opcode >> 16) & 0xF;
    uint8_t op2 = opcode & 0xF;

    uint32_t val;
    bool shift_by_reg = get_bit(opcode, 4);
    uint8_t shift_type = (opcode >> 5) & 0b11;
    if (shift_by_reg)
    {
        uint8_t rs = cpu->reg[(opcode >> 8) & 0xF] & 0xF;
        bool a;
        val = shift(shift_type, cpu->reg[op2], rs, a);
    }
    else
    {
        uint8_t imm = (opcode >> 7) & 0b1'1111;
        bool a;
        val = shift(shift_type, cpu->reg[op2], imm, a);
    }

    cpu->reg[dest] = cpu->reg[op1] + val;

    std::cout << cpu->reg[15] <<  std::hex << " add reg - reg[" << (int)dest << "] = reg[" << (int)op1 << "] + " << val << '\n';
}

void bics_reg(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t op1 = (opcode >> 16) & 0xF;
    uint8_t op2 = opcode & 0xF;

    uint32_t val;
    bool shift_by_reg = get_bit(opcode, 4);
    uint8_t shift_type = (opcode >> 5) & 0b11;

    bool carry;
    if (shift_by_reg)
    {
        uint8_t rs = cpu->reg[(opcode >> 8) & 0xF] & 0xF;
        val = shift(shift_type, cpu->reg[op2], rs, carry);
    }
    else
    {
        uint8_t imm = (opcode >> 7) & 0b1'1111;
        val = shift(shift_type, cpu->reg[op2], imm, carry);
    }

    bool zero = (cpu->reg[op1] & ~val) ? 0 : 1;
    bool sign = (cpu->reg[op1] & ~val) >> 31 ? 1 : 0;

    change_bit(cpu->cpsr, sign, 31);
    change_bit(cpu->cpsr, zero, 30);
    change_bit(cpu->cpsr, carry, 29);
    cpu->reg[dest] = cpu->reg[op1] & ~val;

    std::cout << cpu->reg[15] <<  std::hex << " bics reg - reg[" << (int)dest << "] = reg[" << (int)op1 << "] &~ " << val << '\n';
}

void bx(ARM9 *cpu, uint32_t opcode)
{
    uint32_t addr = cpu->reg[opcode & 0xF];
    if(get_bit(addr, 0))
    {
        //switch to THUMB
        change_bit(cpu->cpsr, 1, 5);
        std::cout << cpu->reg[15] <<  "THUMB NOT IMPLEMENTED\n";
        exit(0);
    }
    cpu->reg[15] = addr & 0xFFFFFFFE;
    std::cout << "bx " << std::hex << addr << '\n';
}

void store_regs(ARM9 *cpu, uint32_t opcode)
{
    uint32_t addr = cpu->reg[(opcode >> 16) & 0xF];
    uint16_t reg_list = opcode & 0xFFFF;

    bool pre_post = get_bit(opcode, 24);
    bool up_down = get_bit(opcode, 23);

    //TODO USER MODE

    uint32_t poz = 0;
    for(int i = 0; i < 16; i++)
    {
        if(get_bit(reg_list, i))
        {
            if(pre_post == 1 && up_down == 1)
            {   
                poz++;
                cpu->mem.write32(addr + poz, cpu->reg[i]);
            }
            if(pre_post == 0 && up_down == 1)
            {   
                cpu->mem.write32(addr + poz, cpu->reg[i]);
                poz++;
            }
            if(pre_post == 1 && up_down == 0)
            {   
                poz--;
                cpu->mem.write32(addr + poz, cpu->reg[i]);
            }
            if(pre_post == 0 && up_down == 0)
            {   
                cpu->mem.write32(addr + poz, cpu->reg[i]);
                poz--;
            }

            
        }
    }
    std::cout << cpu->reg[15] <<  " stm " << std::hex << addr << " " << reg_list;
    std::cout << '\n';
}

void load_regs(ARM9 *cpu, uint32_t opcode)
{
    uint32_t addr = cpu->reg[(opcode >> 16) & 0xF];
    uint16_t reg_list = opcode & 0xFFFF;

    bool pre_post = get_bit(opcode, 24);
    bool up_down = get_bit(opcode, 23);

    //TODO USER MODE

    uint32_t poz = 0;
    for(int i = 0; i < 16; i++)
    {
        if(get_bit(reg_list, i))
        {
            if(pre_post == 0 && up_down == 0)
            {   
                poz++;
                cpu->reg[i] = cpu->mem.read32(addr + poz);
            }
            if(pre_post == 0 && up_down == 1)
            {   
                poz--;
                cpu->reg[i] = cpu->mem.read32(addr + poz);
            }
            if(pre_post == 1 && up_down == 0)
            {   
                cpu->reg[i] = cpu->mem.read32(addr + poz);
                poz++;
            }
            if(pre_post == 1 && up_down == 1)
            {   
                cpu->reg[i] = cpu->mem.read32(addr + poz);
                poz--;
            }            
        }
    }
    std::cout << cpu->reg[15] <<  " ldm " << std::hex << addr << " " << reg_list;
    std::cout << '\n';
}

void subs_imm(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t op1 = (opcode >> 16) & 0xF;

    uint8_t imm = opcode & 0xFF;
    uint8_t shift_amount = (opcode >> 8) & 0xF;

    cpu->reg[dest] = cpu->reg[op1] - rightRotate(imm, shift_amount * 2, 32);

    bool zero = cpu->reg[dest] ? 0 : 1;
    bool sign = cpu->reg[dest] >> 31 ? 1 : 0;

    change_bit(cpu->cpsr, zero, 30);
    change_bit(cpu->cpsr, sign, 31);

    std::cout << cpu->reg[15] <<  std::hex << " subs imm - reg[" << (int)dest << "] = reg[" << (int)op1 << "] - " << rightRotate(imm, shift_amount * 2, 32) << '\n';
}

void subs_reg(ARM9 *cpu, uint32_t opcode)
{
    uint8_t dest = (opcode >> 12) & 0xF;
    uint8_t op1 = (opcode >> 16) & 0xF;
    uint8_t op2 = opcode & 0xF;

    uint32_t val;
    bool shift_by_reg = get_bit(opcode, 4);
    uint8_t shift_type = (opcode >> 5) & 0b11;
    if (shift_by_reg)
    {
        uint8_t rs = cpu->reg[(opcode >> 8) & 0xF] & 0xF;
        bool a;
        val = shift(shift_type, cpu->reg[op2], rs, a);
    }
    else
    {
        uint8_t imm = (opcode >> 7) & 0b1'1111;
        bool a;
        val = shift(shift_type, cpu->reg[op2], imm, a);
    }

    cpu->reg[dest] = cpu->reg[op1] - val;

    bool zero = cpu->reg[dest] ? 0 : 1;
    bool sign = cpu->reg[dest] >> 31 ? 1 : 0;

    change_bit(cpu->cpsr, zero, 30);
    change_bit(cpu->cpsr, sign, 31);

    std::cout << cpu->reg[15] <<  std::hex << " subs reg - reg[" << (int)dest << "] = reg[" << (int)op1 << "] - " << val << '\n';
}