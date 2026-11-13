#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stddef.h>

#define MEM_SIZE 2 << 10

typedef struct
{
  uint32_t words[MEM_SIZE];
} Mem;

void mem_init(Mem *m, const uint32_t *words, size_t n);
uint32_t mem_read(const Mem* m, uint32_t addr);

#endif
