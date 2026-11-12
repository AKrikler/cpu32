#include "alu.h"

AluResult alu(AluOp op, uint32_t a, uint32_t b)
{
  AluResult r;
  r.eq = (a == b);
  r.lt = (int32_t)a < (int32_t)b;
  r.ltu = a < b;

  if (op & 0x8)
  {
    r.y = (op & 0x1) ? r.ltu : r.lt;
    return r;
  }

  uint32_t s = b & 0x1F;
  switch (op)
  {
    case ALU_ADD: r.y = a + b; break;
    case ALU_SUB: r.y = a - b; break;
    case ALU_AND: r.y = a & b; break;
    case ALU_OR:  r.y = a | b; break;
    case ALU_XOR: r.y = a ^ b; break;
    case ALU_SLL: r.y = a << s; break;
    case ALU_SRL: r.y = a >> s; break;
    case ALU_SRA: r.y = (uint32_t)((int32_t)a >> s); break;
    default:      r.y = 0; break;
  }

  return r;
}
