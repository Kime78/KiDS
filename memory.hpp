#pragma once

#include <vector>
#include <memory>
#include <string>
class Memory_ARM9
{
public:
    std::unique_ptr<uint8_t[]> instruction_tcm;
    std::unique_ptr<uint8_t[]> data_tcm;
    std::unique_ptr<uint8_t[]> main_memory;
    std::unique_ptr<uint8_t[]> wram;
    std::unique_ptr<uint8_t[]> palettes;
    std::unique_ptr<uint8_t[]> vram_engineA_bg;
    std::unique_ptr<uint8_t[]> vram_engineB_bg;
    std::unique_ptr<uint8_t[]> vram_engineA_obj;
    std::unique_ptr<uint8_t[]> vram_engineB_obj;
    std::unique_ptr<uint8_t[]> vram_lcdc;
    std::unique_ptr<uint8_t[]> oam;
    std::unique_ptr<uint8_t[]> gba_rom;
    std::unique_ptr<uint8_t[]> gba_ram;
    std::unique_ptr<uint8_t[]> arm9_bios;
    
    std::vector<uint8_t> rom;

    uint8_t read8(uint32_t address);
    uint16_t read16(uint32_t address);
    uint32_t read32(uint32_t address);

    void write8(uint32_t address, uint8_t value);
    void write16(uint32_t address, uint16_t value);
    void write32(uint32_t address, uint32_t value);

    void read_rom(const std::string &path);
    Memory_ARM9();
    ~Memory_ARM9() = default;
};

struct Header
{
    std::string game_title;
    uint32_t game_code;
    uint16_t maker_code;
    uint8_t unit_code;
    uint8_t encryption;
    uint8_t chip_size;
    uint8_t nds_version;
    uint8_t rom_version;
    uint8_t autostart;
    uint32_t arm9_rom_offset;
    uint32_t arm9_entry_address;
    uint32_t arm9_ram_address;
    uint32_t arm9_size;
    //TODO implement the others
};