#ifndef __READ_OPCODE_H_
#define __READ_OPCODE_H_
#include "instruction.h"
#include "opcode.h"

int read_opcode(struct x86_instruction *instr, struct opcode *opcode, unsigned char **src);

#endif
