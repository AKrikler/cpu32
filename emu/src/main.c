#include <stdio.h>
#include <stdlib.h>

#include "alu.h"

int main(int argc, char** argv)
{
  if (argc != 4) return -1;

  AluOp op = strtoul(argv[1], NULL, 16);
  uint32_t a = strtoul(argv[2], NULL, 16);
  uint32_t b = strtol(argv[3], NULL, 16);

  AluResult res = alu(op, a, b);
  printf("0x%08X eq=%d lt=%d ltu=%d\n", res.y, res.eq, res.lt, res.ltu);

  return 0;
}
