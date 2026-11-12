#include "mem.h"

uint8_t mem_read8(const Mem *m, uint32_t addr)
{
  return m->bytes[addr];
}

uint16_t mem_read16(const Mem *m, uint32_t addr)
{
  return ((uint16_t)m->bytes[addr]) |
         ((uint16_t)m->bytes[addr + 1] << 8);
}

uint32_t mem_read32(const Mem *m, uint32_t addr)
{
  return ((uint32_t)m->bytes[addr]) |
         ((uint32_t)m->bytes[addr + 1] << 8) |
         ((uint32_t)m->bytes[addr + 2] << 16) |
         ((uint32_t)m->bytes[addr + 3] << 24);
}

void mem_write8(Mem *m, uint32_t addr, uint8_t val)
{
  m->bytes[addr] = val;
}

void mem_write16(Mem *m, uint32_t addr, uint16_t val)
{
  m->bytes[addr]     = (uint8_t)(val & 0xFF);
  m->bytes[addr + 1] = (uint8_t)((val >> 8) & 0xFF);
}

void mem_write32(Mem *m, uint32_t addr, uint32_t val)
{
  m->bytes[addr]     = (uint8_t)(val & 0xFF);
  m->bytes[addr + 1] = (uint8_t)((val >> 8) & 0xFF);
  m->bytes[addr + 2] = (uint8_t)((val >> 16) & 0xFF);
  m->bytes[addr + 3] = (uint8_t)((val >> 24) & 0xFF);
}
