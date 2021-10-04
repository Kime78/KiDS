#include <iostream>

#include "arm9.hpp"
#include "arm9_instructions.hpp"
#include "math.hpp"

Header get_header(Memory_ARM9 &mem)
{
    Header ret;
    char title[12];

    for (int i = 0; i < 12; i++)
    {
        title[i] = mem.rom[i];
    }

    ret.game_title = title;
    ret.game_code = (mem.rom[0xC + 3] << 24) | (mem.rom[0xC + 2] << 16) | (mem.rom[0xC + 1] << 8) | mem.rom[0xC];
    //TODO fill this
    ret.arm9_rom_offset = (mem.rom[0x20 + 3] << 24) | (mem.rom[0x20 + 2] << 16) | (mem.rom[0x20 + 1] << 8) | mem.rom[0x20];
    ret.arm9_entry_address = (mem.rom[0x24 + 3] << 24) | (mem.rom[0x24 + 2] << 16) | (mem.rom[0x24 + 1] << 8) | mem.rom[0x24];
    ret.arm9_ram_address = (mem.rom[0x28 + 3] << 24) | (mem.rom[0x28 + 2] << 16) | (mem.rom[0x28 + 1] << 8) | mem.rom[0x28];
    ret.arm9_size = (mem.rom[0x2C + 3] << 24) | (mem.rom[0x2C + 2] << 16) | (mem.rom[0x2C + 1] << 8) | mem.rom[0x2C];

    return ret;
}

ARM9::ARM9()
{
    mem.read_rom("roms/armwrestler.nds");
    head = get_header(mem);
    pc = head.arm9_entry_address;
    for (int i = 0; i < head.arm9_size; i++)
    {
        mem.write8(head.arm9_ram_address + i, mem.rom[head.arm9_rom_offset + i]);
    }
    fill_lut();
}

void ARM9::fill_lut()
{
    //fill conditional opcodes
    for (int i = 0; i < 0x1000; i++)
    {
        conditional_instr[i] = undefined_instruction;
        if ((i >> 4) == 0b0001'0010)
            conditional_instr[i] = msr_cpsr_reg;
        if ((i >> 4) == 0b0001'0110)
            conditional_instr[i] = msr_spsr_reg;
        if ((i >> 4) == 0b0011'0010)
            conditional_instr[i] = msr_cpsr_imm;
        if ((i >> 4) == 0b0011'0110)
            conditional_instr[i] = msr_spsr_imm;

        if ((i >> 10) == 0b01)
        {
            if(get_bit(i , 4)) // load bit
            {
                if(get_bit(i, 6)) // byte/word bit
                {
                    if(get_bit(i, 9))
                    {
                        conditional_instr[i] = ldrb_imm;
                    }
                    else
                    {
                        conditional_instr[i] = ldrb_reg;
                    }
                }
                else
                {
                    if(get_bit(i, 9))
                    {
                        conditional_instr[i] = ldr_imm;
                    }
                    else
                    {
                        conditional_instr[i] = ldr_reg;
                    }
                }
            }
            else
            {
                if(get_bit(i, 6)) // byte/word bit
                {
                    if(get_bit(i, 9))
                    {
                        conditional_instr[i] = strb_imm;
                    }
                    else
                    {
                        conditional_instr[i] = strb_reg;
                    }
                }
                else
                {
                    if(get_bit(i, 9))
                    {
                        conditional_instr[i] = str_imm;
                    }
                    else
                    {   
                        conditional_instr[i] = str_reg;
                    }
                }
            }
        }

        if ((i >> 9) == 0b000) //data transfer
        {
            if ((i & 0b1111) == 0b1011)
            {
                if (get_bit(i, 4))
                {
                    //ldrh
                    if (get_bit(i, 6))
                    {
                        //ldrh imm
                    }
                    else
                    {
                        //ldrh reg
                    }
                }
                else
                {
                    //strh
                    if (get_bit(i, 6))
                    {
                        //strh imm
                        conditional_instr[i] = strh_imm;
                    }
                    else
                    {
                        //strh reg
                    }
                }
            }
        }

        if ((i >> 4) == 0b101000)
            conditional_instr[i] = add_imm;
        if ((i >> 4) == 0b111010)
            conditional_instr[i] = mov_imm;
    }

    //fill unconditional opcodes
    for (int i = 0; i < 0x1000; i++)
    {
        unconditional_instr[i] = undefined_instruction;
    }

    //fill thumb opcodes
    for (int i = 0; i < 0x400; i++)
    {
        thumb_instr[i] = undefined_instruction_thumb;
    }
}

void ARM9::step()
{
    uint32_t instr = mem.read32(pc);
    uint32_t opcode = get_opcode(instr);

    //emulate cond field
    uint8_t cond = instr >> 28;
    bool can_execute = 0;
    switch (cond)
    {
    case 0xE:
        can_execute = 1;
        break;

    default:
        std::cout << "[ERROR] COND " << std::hex << (int)cond << "NOT IMPLEMENTED!";
        exit(0);
    }

    if (can_execute)
    {
        conditional_instr[opcode](this, instr);
    }

    if (cond == 0b1111)
    {
        unconditional_instr[opcode](this, instr);
    }
    pc += 4;
}