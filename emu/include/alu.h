#ifndef ALU_H
#define ALU_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
  ALU_ADD, ALU_SUB, ALU_AND, ALU_OR, ALU_XOR,
  ALU_SLL, ALU_SRL, ALU_SRA, ALU_SLT, ALU_SLTU
} AluOp;

typedef struct
{
  uint32_t y;
  bool eq, lt, ltu;
} AluResult;

AluResult alu(AluOp op, uint32_t a, uint32_t b);


#endif
