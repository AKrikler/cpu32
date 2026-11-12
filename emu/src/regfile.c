#include "regfile.h"

uint32_t reg_read(const RegFile rf, uint8_t i)
{
  if (!i) return 0;
  return rf[i];
}

void reg_write(RegFile rf, uint8_t i, uint32_t data)
{
  if (!i) return;
  rf[i] = data;
}
