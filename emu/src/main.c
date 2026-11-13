#include <stdio.h>
#include "cpu.h"

static void cpu_trace(const Cpu *before, const Cpu *after)
{
  printf(before->ex ? "EXECUTE " : "FETCH ");

  printf("%08X: %08X", before->pc, after->ir);

  for (int i = 1; i < NUM_REGS; i++)
    if (before->rf[i] != after->rf[i]) printf("  r%02d=0x%08X", i, after->rf[i]);

  if (after->pc != before->pc) printf("  pc=0x%08X", after->pc);

  if (after->halted) printf("  halted");
  printf("\n");
}

int main(int argc, char **argv)
{
  if (argc != 2) return -1;

  static uint32_t image[MEM_SIZE];
  size_t n = 0;

  FILE *f = fopen(argv[1], "r");
  uint32_t word;
  while (n < MEM_SIZE && fscanf(f, "%x", &word) == 1) image[n++] = word;
  fclose(f);

  Mem mem;
  Cpu cpu;
  mem_init(&mem, image, n);
  cpu_reset(&cpu, &mem);

  while (!cpu.halted)
  {
    Cpu prev = cpu;
    cpu_tick(&cpu);
    cpu_trace(&prev, &cpu);
  }

  for (int i = 0; i < 8; i++) printf("r%02d = 0x%08X\n", i, reg_read(cpu.rf, i));

  return 0;
}
