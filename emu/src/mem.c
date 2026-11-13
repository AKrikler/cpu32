#include <string.h>
#include "mem.h"

void mem_init(Mem *m, const uint32_t *words, size_t n)
{
  memset(m->words, 0, sizeof(m->words));
  n = n > MEM_SIZE ? MEM_SIZE : n;
  memcpy(m->words, words, n << 2);
}

uint32_t mem_read(const Mem *m, uint32_t addr)
{
  return m->words[addr >> 2];
}
