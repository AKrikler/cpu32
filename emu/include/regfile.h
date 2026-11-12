#ifndef REGFILE_H
#define REGFILE_H

#include <stdint.h>

#define NUM_REGS 32

typedef uint32_t RegFile[NUM_REGS];

uint32_t reg_read(const RegFile rf, uint8_t i);
void reg_write(RegFile rf, uint8_t i, uint32_t val);

#endif
