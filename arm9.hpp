#pragma once
#include "memory.hpp"
#include <functional>

#define pc reg[15]
class ARM9
{
public:
   std::function<void(ARM9 *cpu,uint32_t)> conditional_instr[0x1000];
   std::function<void(ARM9 *cpu,uint32_t)> unconditional_instr[0x1000];
   std::function<void(ARM9 *cpu,uint16_t)> thumb_instr[0x400];
   uint32_t reg[16];
   uint32_t cpsr, spsr;
   void step();
   Memory_ARM9 mem;
   Header head;
   ARM9();
   ~ARM9() = default;
   void fill_lut();
};


