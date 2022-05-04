#include <iostream>

#include "memory.hpp"

Memory_ARM9::Memory_ARM9()
{
    instruction_tcm = std::make_unique<uint8_t[]>(32'768);
    data_tcm = std::make_unique<uint8_t[]>(16'384);
    main_memory = std::make_unique<uint8_t[]>(4'194'304);
    wram = std::make_unique<uint8_t[]>(32'768);
    palettes = std::make_unique<uint8_t[]>(2048);
    vram_engineA_bg = std::make_unique<uint8_t[]>(524'288);
    vram_engineB_bg = std::make_unique<uint8_t[]>(131'072);
    vram_engineA_obj = std::make_unique<uint8_t[]>(262'144);
    vram_engineB_obj = std::make_unique<uint8_t[]>(131'072);
    vram_lcdc = std::make_unique<uint8_t[]>(671'744);
    oam = std::make_unique<uint8_t[]>(2048);
    gba_rom = std::make_unique<uint8_t[]>(33'554'432);
    gba_ram = std::make_unique<uint8_t[]>(65'536);
    arm9_bios = std::make_unique<uint8_t[]>(32'768);
}
// TODO LIMIT THE READ/WRITE FUNCTIONS TO THEIR SIZES
void Memory_ARM9::write8(uint32_t address, uint8_t value)
{
    if (address < 0x02000000) //tcm stuff
    {
        std::cout << "[WARNING] TCM NOT IMPLEMENTED!\n";
    }
    if (address >= 0x02000000 && address < 0x03000000)
    {
        main_memory[address - 0x02000000] = value;
    }
    if (address >= 0x03000000 && address < 0x04000000)
    {
        wram[address - 0x03000000] = value;
    }
    if (address >= 0x04000000 && address < 0x05000000) //io stuff
    {
        std::cout << "[WARNING] UNIMPLEMENTED IO 8 BIT WRITE AT " << std::hex << address << " WITH THE VALUE " << value << '\n';
    }
    if (address >= 0x05000000 && address < 0x06000000)
    {
        palettes[address - 0x05000000] = value;
    }
    if (address >= 0x06000000 && address < 0x06200000)
    {
        vram_engineA_bg[address - 0x06000000] = value;
    }
    if (address >= 0x06200000 && address < 0x06400000)
    {
        vram_engineB_bg[address - 0x06200000] = value;
    }
    if (address >= 0x06400000 && address < 0x06600000)
    {
        vram_engineA_obj[address - 0x06400000] = value;
    }
    if (address >= 0x06600000 && address < 0x06800000)
    {
        vram_engineB_obj[address - 0x06600000] = value;
    }
    if (address >= 0x06800000 && address < 0x07000000)
    {
        oam[address - 0x06800000] = value;
    }
    if (address >= 0x07000000 && address < 0x08000000)
    {
        std::cout << "[WARNING] 8 BIT WRITE TO GBA ROM\n";
        gba_rom[address - 0x07000000] = value;
    }
    if (address >= 0x08000000 && address < 0x0A000000)
    {
        gba_ram[address - 0x08000000] = value;
    }
    if (address >= 0xFFFF0000 && address < 0xFFFFFFFF)
    {
        arm9_bios[address - 0xFFFF0000] = value;
    }
}

uint8_t Memory_ARM9::read8(uint32_t address)
{
    if (address < 0x02000000) //tcm stuff
    {
        std::cout << "[WARNING] TCM NOT IMPLEMENTED!\n";
    }
    if (address >= 0x02000000 && address < 0x03000000)
    {
        return main_memory[address - 0x02000000];
    }
    if (address >= 0x03000000 && address < 0x04000000)
    {
        return wram[address - 0x03000000];
    }
    if (address >= 0x04000000 && address < 0x05000000) //io stuff
    {
        std::cout << "[WARNING] UNIMPLEMENTED 8 BIT IO READ AT " << std::hex << address << '\n';
    }
    if (address >= 0x05000000 && address < 0x06000000)
    {
        return palettes[address - 0x05000000];
    }
    if (address >= 0x06000000 && address < 0x06200000)
    {
        return vram_engineA_bg[address - 0x06000000];
    }
    if (address >= 0x06200000 && address < 0x06400000)
    {
        return vram_engineB_bg[address - 0x06200000];
    }
    if (address >= 0x06400000 && address < 0x06600000)
    {
        return vram_engineA_obj[address - 0x06400000];
    }
    if (address >= 0x06600000 && address < 0x06800000)
    {
        return vram_engineB_obj[address - 0x06600000];
    }
    if (address >= 0x06800000 && address < 0x07000000)
    {
        return oam[address - 0x06800000];
    }
    if (address >= 0x07000000 && address < 0x08000000)
    {
        return gba_rom[address - 0x07000000];
    }
    if (address >= 0x08000000 && address < 0x0A000000)
    {
        return gba_ram[address - 0x08000000];
    }
    if (address >= 0xFFFF0000 && address < 0xFFFFFFFF)
    {
        return arm9_bios[address - 0xFFFF0000];
    }
}
void Memory_ARM9::write16(uint32_t address, uint16_t value)
{
    uint8_t byte1 = value & 0xFF;
    uint8_t byte2 = value >> 8;
    if (address < 0x02000000) //tcm stuff
    {
        std::cout << "[WARNING] TCM NOT IMPLEMENTED!\n";
    }
    if (address >= 0x02000000 && address < 0x03000000)
    {
        main_memory[address - 0x02000000] = byte1;
        main_memory[address - 0x02000000 + 1] = byte2;
    }
    if (address >= 0x03000000 && address < 0x04000000)
    {
        wram[address - 0x03000000] = byte1;
        wram[address - 0x03000000 + 1] = byte2;
    }
    if (address >= 0x04000000 && address < 0x05000000) //io stuff
    {
        std::cout << "[WARNING] UNIMPLEMENTED 16 BIT IO WRITE AT " << std::hex << address << " WITH THE VALUE " << value << '\n';
    }
    if (address >= 0x05000000 && address < 0x06000000)
    {
        palettes[address - 0x05000000] = byte1;
        palettes[address - 0x05000000 + 1] = byte2;
    }
    if (address >= 0x06000000 && address < 0x06200000)
    {
        vram_engineA_bg[address - 0x06000000] = byte1;
        vram_engineA_bg[address - 0x06000000 + 1] = byte2;
    }
    if (address >= 0x06200000 && address < 0x06400000)
    {
        vram_engineB_bg[address - 0x06200000] = byte1;
        vram_engineB_bg[address - 0x06200000 + 1] = byte2;
    }
    if (address >= 0x06400000 && address < 0x06600000)
    {
        vram_engineA_obj[address - 0x06400000] = byte1;
        vram_engineA_obj[address - 0x06400000 + 1] = byte2;
    }
    if (address >= 0x06600000 && address < 0x06800000)
    {
        vram_engineB_obj[address - 0x06600000] = byte1;
        vram_engineB_obj[address - 0x06600000 + 1] = byte2;
    }
    if (address >= 0x06800000 && address < 0x07000000)
    {
        oam[address - 0x06800000] = byte1;
        oam[address - 0x06800000 + 1] = byte2;
    }
    if (address >= 0x07000000 && address < 0x08000000)
    {
        std::cout << "[WARNING] WRITE TO GBA ROM\n";
        gba_rom[address - 0x07000000] = byte1;
        gba_rom[address - 0x07000000 + 1] = byte2;
    }
    if (address >= 0x08000000 && address < 0x0A000000)
    {
        gba_ram[address - 0x08000000] = byte1;
        gba_ram[address - 0x08000000 + 1] = byte2;
    }
    if (address >= 0xFFFF0000 && address < 0xFFFFFFFF)
    {
        arm9_bios[address - 0xFFFF0000] = byte1;
        arm9_bios[address - 0xFFFF0000 + 1] = byte2;
    }
}

uint16_t Memory_ARM9::read16(uint32_t address)
{
    if (address < 0x02000000) //tcm stuff
    {
        std::cout << "[WARNING] TCM NOT IMPLEMENTED!\n";
    }
    if (address >= 0x02000000 && address < 0x03000000)
    {
        return (main_memory[address - 0x02000000 + 1] << 8) | main_memory[address - 0x02000000];
    }
    if (address >= 0x03000000 && address < 0x04000000)
    {
        return (wram[address - 0x03000000 + 1] << 8) | wram[address - 0x03000000];
    }
    if (address >= 0x04000000 && address < 0x05000000) //io stuff
    {
        switch (address) {
            case 0x4000004: {
                std::cout << "[LOG] READ DISPSTAT WITH VALUE: " << status_regs.DISPSTAT << std::endl;
                return status_regs.DISPSTAT;
                break;
            }
            default: {
                std::cout << "[WARNING] UNIMPLEMENTED IO 16 BIT READ AT " << std::hex << address << '\n';
                break;
            }
        }
    }
    if (address >= 0x05000000 && address < 0x06000000)
    {
        return (palettes[address - 0x05000000 + 1] << 8) | palettes[address - 0x05000000];
    }
    if (address >= 0x06000000 && address < 0x06200000)
    {
        return (vram_engineA_bg[address - 0x06000000 + 1] << 8) | main_memory[address - 0x06000000];
    }
    if (address >= 0x06200000 && address < 0x06400000)
    {
        return (vram_engineB_bg[address - 0x06200000 + 1] << 8) | vram_engineB_bg[address - 0x06200000];
    }
    if (address >= 0x06400000 && address < 0x06600000)
    {
        return (vram_engineA_obj[address - 0x06400000 + 1] << 8) | vram_engineA_obj[address - 0x06400000];
    }
    if (address >= 0x06600000 && address < 0x06800000)
    {
        return (vram_engineB_obj[address - 0x06600000 + 1] << 8) | vram_engineB_obj[address - 0x06600000];
    }
    if (address >= 0x06800000 && address < 0x07000000)
    {
        return (oam[address - 0x06800000 + 1] << 8) | oam[address - 0x06800000];
    }
    if (address >= 0x07000000 && address < 0x08000000)
    {
        return (gba_rom[address - 0x07000000 + 1] << 8) | gba_rom[address - 0x07000000];
    }
    if (address >= 0x08000000 && address < 0x0A000000)
    {
        return (gba_ram[address - 0x08000000 + 1] << 8) | gba_ram[address - 0x08000000];
    }
    if (address >= 0xFFFF0000 && address < 0xFFFFFFFF)
    {
        return (arm9_bios[address - 0xFFFF0000 + 1] << 8) | arm9_bios[address - 0xFFFF0000];
    }
}

void Memory_ARM9::write32(uint32_t address, uint32_t value)
{
    uint8_t byte1 = value & 0xFF;
    uint8_t byte2 = (value >> 8) & 0xFF;
    uint8_t byte3 = (value >> 16) & 0xFF;
    uint8_t byte4 = (value >> 24) & 0xFF;

    if (address < 0x02000000) //tcm stuff
    {
        std::cout << "[WARNING] TCM NOT IMPLEMENTED!\n";
    }
    if (address >= 0x02000000 && address < 0x03000000)
    {
        main_memory[address - 0x02000000] = byte1;
        main_memory[address - 0x02000000 + 1] = byte2;
        main_memory[address - 0x02000000 + 2] = byte3;
        main_memory[address - 0x02000000 + 3] = byte4;
    }
    if (address >= 0x03000000 && address < 0x04000000)
    {
        wram[address - 0x03000000] = byte1;
        wram[address - 0x03000000 + 1] = byte2;
        wram[address - 0x03000000 + 2] = byte3;
        wram[address - 0x03000000 + 3] = byte4;
    }
    if (address >= 0x04000000 && address < 0x05000000) //io stuff
    {
        std::cout << "[WARNING] UNIMPLEMENTED IO 32 BIT WRITE AT " << std::hex << address << " WITH THE VALUE " << value << '\n';
    }
    if (address >= 0x05000000 && address < 0x06000000)
    {
        palettes[address - 0x05000000] = byte1;
        palettes[address - 0x05000000 + 1] = byte2;
        palettes[address - 0x05000000 + 2] = byte3;
        palettes[address - 0x05000000 + 3] = byte4;
    }
    if (address >= 0x06000000 && address < 0x06200000)
    {
        vram_engineA_bg[address - 0x06000000] = byte1;
        vram_engineA_bg[address - 0x06000000 + 1] = byte2;
        vram_engineA_bg[address - 0x06000000 + 2] = byte3;
        vram_engineA_bg[address - 0x06000000 + 3] = byte4;
    }
    if (address >= 0x06200000 && address < 0x06400000)
    {
        vram_engineB_bg[address - 0x06200000] = byte1;
        vram_engineB_bg[address - 0x06200000 + 1] = byte2;
        vram_engineB_bg[address - 0x06200000 + 2] = byte3;
        vram_engineB_bg[address - 0x06200000 + 3] = byte4;
    }
    if (address >= 0x06400000 && address < 0x06600000)
    {
        vram_engineA_obj[address - 0x06400000] = byte1;
        vram_engineA_obj[address - 0x06400000 + 1] = byte2;
        vram_engineA_obj[address - 0x06400000 + 2] = byte3;
        vram_engineA_obj[address - 0x06400000 + 3] = byte4;
    }
    if (address >= 0x06600000 && address < 0x06800000)
    {
        vram_engineB_obj[address - 0x06600000] = byte1;
        vram_engineB_obj[address - 0x06600000 + 1] = byte2;
        vram_engineB_obj[address - 0x06600000 + 2] = byte3;
        vram_engineB_obj[address - 0x06600000 + 3] = byte4;
    }
    if (address >= 0x06800000 && address < 0x07000000)
    {
        oam[address - 0x06800000] = byte1;
        oam[address - 0x06800000 + 1] = byte2;
        oam[address - 0x06800000 + 2] = byte3;
        oam[address - 0x06800000 + 3] = byte4;
    }
    if (address >= 0x07000000 && address < 0x08000000)
    {
        std::cout << "[WARNING] WRITE TO GBA ROM\n";
        gba_rom[address - 0x07000000] = byte1;
        gba_rom[address - 0x07000000 + 1] = byte2;
        gba_rom[address - 0x07000000 + 2] = byte3;
        gba_rom[address - 0x07000000 + 3] = byte4;
    }
    if (address >= 0x08000000 && address < 0x0A000000)
    {
        gba_ram[address - 0x08000000] = byte1;
        gba_ram[address - 0x08000000 + 1] = byte2;
        gba_ram[address - 0x08000000 + 2] = byte3;
        gba_ram[address - 0x08000000 + 3] = byte4;
    }
    if (address >= 0xFFFF0000 && address < 0xFFFFFFFF)
    {
        arm9_bios[address - 0xFFFF0000] = byte1;
        arm9_bios[address - 0xFFFF0000 + 1] = byte2;
        arm9_bios[address - 0xFFFF0000 + 2] = byte3;
        arm9_bios[address - 0xFFFF0000 + 3] = byte4;
    }
}

uint32_t Memory_ARM9::read32(uint32_t address)
{
    if (address < 0x02000000) //tcm stuff
    {
        std::cout << "[WARNING] TCM NOT IMPLEMENTED!\n";
    }
    if (address >= 0x02000000 && address < 0x03000000)
    {
        return (main_memory[address - 0x02000000 + 3] << 24) | (main_memory[address - 0x02000000 + 2] << 16) 
        | (main_memory[address - 0x02000000 + 1] << 8) | main_memory[address - 0x02000000];
    }
    if (address >= 0x03000000 && address < 0x04000000)
    {
        return (wram[address - 0x03000000 + 3] << 24) | (wram[address - 0x03000000 + 2] << 16) 
        | (wram[address - 0x03000000 + 1] << 8) | wram[address - 0x03000000];
    }
    if (address >= 0x04000000 && address < 0x05000000) //io stuff
    {
        std::cout << "[WARNING] UNIMPLEMENTED IO 32 BIT READ AT " << std::hex << address << '\n';
    }
    if (address >= 0x05000000 && address < 0x06000000)
    {
        return (palettes[address - 0x05000000 + 3] << 24) | (palettes[address - 0x05000000 + 2] << 16) 
        | (palettes[address - 0x05000000 + 1] << 8) | palettes[address - 0x05000000];
    }
    if (address >= 0x06000000 && address < 0x06200000)
    {
        return (vram_engineA_bg[address - 0x06000000 + 3] << 24) | (vram_engineA_bg[address - 0x06000000 + 2] << 16) 
        | (vram_engineA_bg[address - 0x06000000 + 1] << 8) | vram_engineA_bg[address - 0x06000000];
    }
    if (address >= 0x06200000 && address < 0x06400000)
    {
        return (vram_engineB_bg[address - 0x06200000 + 3] << 24) | (vram_engineB_bg[address - 0x06200000 + 2] << 16) 
        | (vram_engineB_bg[address - 0x06200000 + 1] << 8) | vram_engineB_bg[address - 0x06200000];
    }
    if (address >= 0x06400000 && address < 0x06600000)
    {
        return (vram_engineA_obj[address - 0x06400000 + 3] << 24) | (vram_engineA_obj[address - 0x06400000 + 2] << 16) 
        | (vram_engineA_obj[address - 0x06400000 + 1] << 8) | vram_engineA_obj[address - 0x06400000];
    }
    if (address >= 0x06600000 && address < 0x06800000)
    {
        return (vram_engineB_obj[address - 0x06600000 + 3] << 24) | (vram_engineB_obj[address - 0x06600000 + 2] << 16) 
        | (vram_engineB_obj[address - 0x06600000 + 1] << 8) | vram_engineB_obj[address - 0x06600000];
    }
    if (address >= 0x06800000 && address < 0x07000000)
    {
        return (oam[address - 0x06800000 + 3] << 24) | (oam[address - 0x06800000 + 2] << 16) 
        | (oam[address - 0x06800000+ 1] << 8) | oam[address - 0x06800000];
    }
    if (address >= 0x07000000 && address < 0x08000000)
    {
        return (gba_rom[address - 0x07000000 + 3] << 24) | (gba_rom[address - 0x07000000 + 2] << 16) 
        | (gba_rom[address - 0x07000000 + 1] << 8) | gba_rom[address - 0x07000000];
    }
    if (address >= 0x08000000 && address < 0x0A000000)
    {
        return (gba_ram[address - 0x08000000 + 3] << 24) | (gba_ram[address - 0x08000000 + 2] << 16) 
        | (gba_ram[address - 0x08000000 + 1] << 8) | gba_ram[address - 0x08000000];
    }
    if (address >= 0xFFFF0000 && address < 0xFFFFFFFF)
    {
        return (arm9_bios[address - 0xFFFF0000 + 3] << 24) | (arm9_bios[address - 0xFFFF0000 + 2] << 16) 
        | (arm9_bios[address - 0xFFFF0000 + 1] << 8) | arm9_bios[address - 0xFFFF0000];
    }
}

void Memory_ARM9::read_rom(const std::string &path)
{
    //rom = LoadBin<uint8_t>(path);
    rom.resize(0xFFFFFFF);
    FILE *file = fopen(path.c_str(), "rb");
    int pos = 0;
    
    while (fread(&rom[pos], 1, 1, file))
    {
       pos++;
    }
    rom.shrink_to_fit();
}

