#ifndef __ADHESIVEDIS_H_
#define __ADHESIVEDIS_H_
#include "opcode.h"
#include "instruction.h"
#include "buildstr.h"

int parse_instruction(struct x86_instruction *instr, unsigned char *src);
void print_instruction(struct x86_instruction *instr);

#endif // ADHESIVEDIS_H_INCLUDED
