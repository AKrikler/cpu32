#include "cpu.h"
#include "alu.h"

static const uint16_t ctrl_words[64] = 
{
  [0x00] = 0x6004,   /* ALU-R */
  [0x01] = 0x2044,   /* ADDI  */
  [0x02] = 0x2444,   /* ANDI  */
  [0x03] = 0x2644,   /* ORI   */
  [0x04] = 0x2844,   /* XORI  */
  [0x05] = 0x3044,   /* SLTI  */
  [0x06] = 0x3244,   /* SLTIU */
  [0x07] = 0x2A44,   /* SLLI  */
  [0x08] = 0x2C44,   /* SRLI  */
  [0x09] = 0x2E44,   /* SRAI  */
  [0x0A] = 0x2154,   /* LUI   */
};

typedef struct
{
  bool ir_we, pc_we, rf_we, v, alur;
  uint8_t a_sel, b_sel, imm_sel, op;
} Ctrl;

static Ctrl control(bool ex, bool halted, uint8_t opcode)
{
  uint16_t word = ex ? ctrl_words[opcode] : 0x202B;

  Ctrl c;
  c.ir_we   = (word & 1) && !halted;
  c.pc_we   = ((word >> 1) & 1) && !halted;
  c.rf_we   = ((word >> 2) & 1) && !halted;
  c.a_sel   = (word >> 3) & 3;
  c.b_sel   = (word >> 5) & 3;
  c.imm_sel = (word >> 7) & 3;
  c.op      = (word >> 9) & 0xF;
  c.v       = (word >> 13) & 1;
  c.alur    = (word >> 14) & 1;
  return c;
}

static uint32_t imm_gen(uint32_t ir, uint8_t sel)
{
  uint32_t imm16 = (uint32_t)(int32_t)(int16_t)(ir & 0xFFFF);
  uint32_t imm21 = ir & 0x1FFFFF;
  if (imm21 & 0x100000) imm21 |= 0xFFE00000;

  switch (sel)
  {
    case 0:  return imm16;
    case 1:  return imm16 << 2;
    case 2:  return (ir & 0x1FFFFF) << 11;
    default: return imm21 << 2;
  }
}

void cpu_reset(Cpu *cpu, Mem *mem)
{
  *cpu = (Cpu){0};
  cpu->mem = mem;
  cpu->pc = 0; // TODO: Determine reset vector
}

void cpu_tick(Cpu *cpu)
{
  uint32_t ir = cpu->ir;
  uint8_t opcode = ir >> 26;
  uint8_t w = (ir >> 21) & 0x1F;
  uint8_t r1 = (ir >> 16) & 0x1F;
  uint8_t r2 = (ir >> 11) & 0x1F;
  uint8_t func = ir & 0xF;

  Ctrl c = control(cpu->ex, cpu->halted, opcode);

  uint32_t o1 = reg_read(cpu->rf, r1);
  uint32_t o2 = reg_read(cpu->rf, r2);
  uint32_t imm = imm_gen(ir, c.imm_sel);

  uint32_t a = c.a_sel == 0 ? o1 : c.a_sel == 1 ? cpu->pc : 0;
  uint32_t b = c.b_sel == 0 ? o2 : c.b_sel == 1 ? 4 : imm;
  uint32_t y = alu((AluOp)(c.alur ? func : c.op), a, b).y;

  uint32_t fetched = mem_read(cpu->mem, cpu->pc);

  bool was_ex = cpu->ex;
  if (c.ir_we) cpu->ir = fetched;
  if (c.pc_we) cpu->pc = y;
  if (c.rf_we) reg_write(cpu->rf, w, y);
  if (was_ex && !c.v) cpu->halted = true;
  cpu->ex = !was_ex;
}
