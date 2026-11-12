#include <stdio.h>
#include "cpu.h"

static void cpu_trace(const Cpu *before, const Cpu *after, uint32_t ir)
{
  printf("%08X: %08X", before->pc, ir);

  for (int i = 1; i < NUM_REGS; i++)
    if (before->rf[i] != after->rf[i]) printf("  r%02d=0x%08X", i, after->rf[i]);

  if (after->pc != before->pc + 4) printf("  pc=0x%08X", after->pc);

  if (after->halted) printf("  halted");
  printf("\n");
}

int main(int argc, char **argv)
{
  Cpu cpu;
  static Mem mem;

  FILE *f = fopen(argv[1], "r");
  uint32_t word, addr = 0;
  while (fscanf(f, "%x", &word) == 1)
  {
    mem_write32(&mem, addr, word);
    addr += 4;
  }
  fclose(f);

  cpu_reset(&cpu, &mem);

  while (!cpu.halted)
  {
    Cpu prev = cpu;
    uint32_t ir = mem_read32(&mem, cpu.pc);
    cpu_step(&cpu);
    cpu_trace(&prev, &cpu, ir);
  }

  for (int i = 0; i < 8; i++) printf("r%02d = 0x%08X\n", i, reg_read(cpu.rf, i));

  return 0;
}
