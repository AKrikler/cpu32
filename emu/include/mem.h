#ifndef MEM_H
#define MEM_H

#include <stdint.h>

#define MEM_SIZE (1u << 20)

typedef struct
{
  uint8_t bytes[MEM_SIZE];
} Mem;

uint8_t  mem_read8 (const Mem* m, uint32_t addr);
uint16_t mem_read16(const Mem* m, uint32_t addr);
uint32_t mem_read32(const Mem* m, uint32_t addr);

void mem_write8 (Mem* m, uint32_t addr, uint8_t  val);
void mem_write16(Mem* m, uint32_t addr, uint16_t val);
void mem_write32(Mem* m, uint32_t addr, uint32_t val);

#endif
