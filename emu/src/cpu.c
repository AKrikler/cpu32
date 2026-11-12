#include "cpu.h"
#include "alu.h"

static const AluOp imm_op[] = { ALU_ADD, ALU_AND, ALU_OR, ALU_XOR, ALU_SLT, ALU_SLTU, ALU_SLL, ALU_SRL, ALU_SRA };

static int32_t sext16(uint32_t x)
{
  return (int16_t)x;
}

void cpu_reset(Cpu *cpu, Mem *mem)
{
  *cpu = (Cpu){0};
  cpu->mem = mem;
  cpu->pc = 0; // TODO: Determine reset vector
}

void cpu_step(Cpu *cpu)
{
  uint32_t pc = cpu->pc;
  uint32_t ir = mem_read32(cpu->mem, pc);

  uint32_t opcode = ir >> 26;
  uint32_t rd = (ir >> 21) & 0x1F;
  uint32_t rs1 = (ir >> 16) & 0x1F;
  uint32_t rs2 = (ir >> 11) & 0x1F;
  uint32_t func = ir & 0x7FF;
  uint32_t imm16 = ir & 0xFFFF;
  uint32_t imm21 = ir & 0x1FFFFF;

  cpu->pc += 4;

  switch (opcode)
  {
    case 0x00:
      reg_write(cpu->rf, rd,
                alu((AluOp)(func & 0xF), reg_read(cpu->rf, rs1), reg_read(cpu->rf, rs2)).y);
      break;
    case 0x01: case 0x02: case 0x03: case 0x04: case 0x05:
    case 0x06: case 0x07: case 0x08: case 0x09:
      reg_write(cpu->rf, rd,
                alu(imm_op[opcode - 1], reg_read(cpu->rf, rs1), (uint32_t)sext16(imm16)).y);
      break;
    case 0x0A:
      reg_write(cpu->rf, rd, imm21 << 11);
      break;
    case 0x33:
    default:
      cpu->halted = true;
      break;
  }
}
