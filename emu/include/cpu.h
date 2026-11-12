#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include "regfile.h"
#include "mem.h"

typedef struct
{
  RegFile rf;
  uint32_t pc;
  bool halted;
  Mem* mem;
} Cpu;

void cpu_reset(Cpu *cpu, Mem *mem);
void cpu_step(Cpu *cpu);

#endif
