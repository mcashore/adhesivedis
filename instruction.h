#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED
#include "opcode.h"

enum operand_flags
{
    HAS_POINTER = (1 << 0), /* The operand is a pointer */
    HAS_REGISTER = (1 << 1), /* The operand contains a register */
    HAS_DISPLACEMENT = (1 << 2), /* The operand has an 8-bit displacement */
    HAS_IMMEDIATE = (1 << 3),   /* The operand has an immediate value */
    HAS_SIB = (1 << 4), /* the operand has an SIB byte*/
    HAS_SEGMENT = (1 << 5), /* has a segment register */
    HAS_REL_IMM = (1 << 6), /* the immediate value is a relative offset */
    HAS_CONST_1 = (1 << 7)  /* the operand is a constant value of 1 */
};

struct modrm {
    unsigned rm : 3;
    unsigned reg : 3;
    unsigned mod : 2;
};

struct sib {
    unsigned base : 3;
    unsigned index : 3;
    unsigned scale : 2;
};

struct operand {
    /* info about the operand - see enum operand_flags */
    unsigned char flags;

    enum mode reg_mode;
    /* which register does this operand use? (if any) */
    unsigned char reg;

    /* how big is the displacement? (if there is a displacement) */
    enum mode disp_mode;

    /* the value of the displacement (the variables correspond to diff sizes) */
    union {
        unsigned long disp32;
        unsigned short disp16;
        unsigned char disp8;
    } disp;

    /* how big is the immediate value? (if any) */
    enum mode imm_mode;

    /* the immediate value (same as displacement..)*/
    union {
        unsigned long imm32;
        unsigned short imm16;
        unsigned char imm8;
    } imm;

    /* the SIB byte (if present and relevant to this operand) */
    struct sib sib;
};

#ifndef MAX_PREFIX
#define MAX_PREFIX   	4
#endif

#ifndef MAX_OPCODE
#define MAX_OPCODE 	    1
#endif

#ifndef MAX_OPERAND
#define MAX_OPERAND     2 /* for now */
#endif

struct x86_instruction {

	unsigned char prefix[MAX_PREFIX];
    enum mode operand_mode;
    enum mode address_mode;

	unsigned char opcode_len;	/*how many bytes in the opcode itself */
	unsigned char opcode_bytes[MAX_OPCODE];

    unsigned char number_of_operands;
	struct operand operands[MAX_OPERAND];

	unsigned char length;
	int opcode;
};

#endif // INSTRUCTION_H_INCLUDED
