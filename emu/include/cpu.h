#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include "regfile.h"
#include "mem.h"

typedef struct
{
  RegFile rf;
  uint32_t pc;
  uint32_t ir;
  uint32_t ipc;
  uint8_t state;
  bool halted;
  Mem *mem;
} Cpu;

void cpu_reset(Cpu *cpu, Mem *mem);
void cpu_tick(Cpu *cpu);

#endif
