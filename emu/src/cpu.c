#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "alu.h"

enum { ST_FETCH, ST_EXEC, ST_EXEC2 };

static uint32_t ctrl_words[3][64];
static bool rom_loaded = false;

typedef union
{
  uint32_t raw;
  struct
  {
    uint32_t ir_we   : 1;
    uint32_t pc_we   : 1;
    uint32_t rf_we   : 1;
    uint32_t a_sel   : 2;
    uint32_t b_sel   : 2;
    uint32_t imm_sel : 2;
    uint32_t op      : 4;
    uint32_t v       : 1;
    uint32_t alur    : 1;
    uint32_t nxt     : 2;
  };
} Ctrl;

static void load_ctrl_rom(const char *path)
{
  if (rom_loaded) return;

  FILE *f = fopen(path, "r");
  if (!f) { perror("Failed to open ctrl.rom"); exit(1); }

  char header[64];
  if (!fgets(header, sizeof(header), f))
  {
    fclose(f);
    perror("Failed to read ctrl.rom header");
    exit(1);
  }

  for (int state = 0; state < 3; state++)
  {
    for (int op = 0; op < 64; op++)
    {
      if (fscanf(f, "%x", &ctrl_words[state][op]) != 1)
      {
        fclose(f);
        perror("Failed to read ctrl.rom data");
        exit(1);
      }
    }
  }

  fclose(f);
  rom_loaded = true;
}

static Ctrl control(uint8_t state, bool halted, uint8_t opcode)
{
  Ctrl c;
  c.raw = ctrl_words[state][opcode];

  if (halted)
  {
    c.ir_we = 0;
    c.pc_we = 0;
    c.rf_we = 0;
  }

  return c;
}

static bool branch(uint8_t opcode, AluResult f)
{
  uint8_t cond = (opcode >> 1) & 3;
  bool hit = cond == 0 ? f.eq : cond == 1 ? f.lt : cond == 2 ? f.ltu : false;
  return hit ^ (opcode & 1);
}

static uint32_t sext(uint32_t x, int bits)
{
  uint32_t m = 1u << (bits - 1);
  return (x ^ m) - m;
}

static uint32_t imm_gen(uint32_t ir, uint8_t sel)
{
  uint32_t i16 = sext(ir & 0xFFFF, 16);
  uint32_t b16 = sext((((ir >> 21) & 0x1F) << 11) | (ir & 0x7FF), 16);
  uint32_t u21 = ir & 0x1FFFFF;

  switch (sel)
  {
    case 0:  return i16;
    case 1:  return b16 << 2;
    case 2:  return u21 << 11;
    default: return sext(u21, 21) << 2;
  }
}

void cpu_reset(Cpu *cpu, Mem *mem)
{
  load_ctrl_rom("../hardware/ctrl.rom");
  *cpu = (Cpu){0};
  cpu->mem = mem;
}

void cpu_tick(Cpu *cpu)
{
  uint32_t ir = cpu->ir;
  uint8_t opcode = ir >> 26;
  uint8_t w = (ir >> 21) & 0x1F;
  uint8_t r1 = (ir >> 16) & 0x1F;
  uint8_t r2 = (ir >> 11) & 0x1F;
  uint8_t func = ir & 0xF;

  Ctrl c = control(cpu->state, cpu->halted, opcode);

  uint32_t o1 = reg_read(cpu->rf, r1);
  uint32_t o2 = reg_read(cpu->rf, r2);
  uint32_t imm = imm_gen(ir, c.imm_sel);

  uint32_t a = c.a_sel == 0 ? o1 : c.a_sel == 1 ? cpu->pc : c.a_sel == 2 ? 0 : cpu->ipc;
  uint32_t b = c.b_sel == 0 ? o2 : c.b_sel == 1 ? 4 : imm;
  AluResult r = alu((AluOp)(c.alur ? func : c.op), a, b);

  uint8_t next = c.nxt == 3 ? (branch(opcode, r) ? ST_EXEC2 : ST_FETCH) : c.nxt;
  uint32_t fetched = mem_read(cpu->mem, cpu->pc);

  if (c.ir_we) { cpu->ir = fetched; cpu->ipc = cpu->pc; }
  if (c.pc_we) {
    printf("JUMPING FROM %08X TO %08X (opcode %02X)\n", cpu->pc, r.y, opcode);
    cpu->pc = r.y;
  }
  if (c.rf_we) reg_write(cpu->rf, w, r.y);
  if (!c.v) cpu->halted = true;
  cpu->state = next;
}
