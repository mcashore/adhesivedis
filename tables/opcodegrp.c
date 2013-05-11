#include "../opcode.h"
/*
struct opcode_group_one_byte
{
    unsigned char byte;  the byte of the main opcode
    unsigned char reg;  what must modrm->reg be ?
    enum mode mode;  default operand mode

    unsigned short operand1;  flags
    unsigned short operand2;  flags
	enum opcodes opcode; / constant for mnemonics
};

*/

struct opcode_group_one_byte one_byte_opcode_groups[] =
{
    /** okay 0x80 and 0x81 are COMPLETELY EQUIVALENT to 0x82 and 0x83, respectively.
     * Instead of using this to my advantage and coming up with some table that
     * reduces memory usage, I just duplicated them.
     * I might do thatl ater, but premature optimization is never a good idea. */
    {0x80, 0, MODE_8, FLG_RM, FLG_IMM8, OPC_ADD },
    {0x80, 1, MODE_8, FLG_RM, FLG_IMM8, OPC_OR },
    {0x80, 2, MODE_8, FLG_RM, FLG_IMM8, OPC_ADC },
    {0x80, 3, MODE_8, FLG_RM, FLG_IMM8, OPC_SBB },
    {0x80, 4, MODE_8, FLG_RM, FLG_IMM8, OPC_AND },
    {0x80, 5, MODE_8, FLG_RM, FLG_IMM8, OPC_SUB },
    {0x80, 6, MODE_8, FLG_RM, FLG_IMM8, OPC_XOR },
    {0x80, 7, MODE_8, FLG_RM, FLG_IMM8, OPC_CMP },
    {0x80, 0, MODE_8, FLG_RM, FLG_IMM8, OPC_ADD },

    {0x81, 0, MODE_DEFAULT, FLG_RM, FLG_IMM, OPC_ADD },
    {0x81, 1, MODE_DEFAULT, FLG_RM, FLG_IMM, OPC_OR },
    {0x81, 2, MODE_DEFAULT, FLG_RM, FLG_IMM, OPC_ADC },
    {0x81, 3, MODE_DEFAULT, FLG_RM, FLG_IMM, OPC_SBB },
    {0x81, 4, MODE_DEFAULT, FLG_RM, FLG_IMM, OPC_AND },
    {0x81, 5, MODE_DEFAULT, FLG_RM, FLG_IMM, OPC_SUB },
    {0x81, 6, MODE_DEFAULT, FLG_RM, FLG_IMM, OPC_XOR },
    {0x81, 7, MODE_DEFAULT, FLG_RM, FLG_IMM, OPC_CMP },

    {0x82, 0, MODE_8, FLG_RM, FLG_IMM8, OPC_ADD },
    {0x82, 1, MODE_8, FLG_RM, FLG_IMM8, OPC_OR },
    {0x82, 2, MODE_8, FLG_RM, FLG_IMM8, OPC_ADC },
    {0x82, 3, MODE_8, FLG_RM, FLG_IMM8, OPC_SBB },
    {0x82, 4, MODE_8, FLG_RM, FLG_IMM8, OPC_AND },
    {0x82, 5, MODE_8, FLG_RM, FLG_IMM8, OPC_SUB },
    {0x82, 6, MODE_8, FLG_RM, FLG_IMM8, OPC_XOR },
    {0x82, 7, MODE_8, FLG_RM, FLG_IMM8, OPC_CMP },
    {0x82, 0, MODE_8, FLG_RM, FLG_IMM8, OPC_ADD },

    {0x83, 0, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_ADD },
    {0x83, 1, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_OR },
    {0x83, 2, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_ADC },
    {0x83, 3, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_SBB },
    {0x83, 4, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_AND },
    {0x83, 5, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_SUB },
    {0x83, 6, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_XOR },
    {0x83, 7, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_CMP },

    {0xC0, 0, MODE_8, FLG_RM, FLG_IMM8, OPC_ROL},
    {0xC0, 1, MODE_8, FLG_RM, FLG_IMM8, OPC_ROR},
    {0xC0, 2, MODE_8, FLG_RM, FLG_IMM8, OPC_RCL},
    {0xC0, 3, MODE_8, FLG_RM, FLG_IMM8, OPC_RCR},
    {0xC0, 4, MODE_8, FLG_RM, FLG_IMM8, OPC_SHL},
    {0xC0, 5, MODE_8, FLG_RM, FLG_IMM8, OPC_SHR},
    {0xC0, 6, MODE_8, FLG_RM, FLG_IMM8, OPC_SAL},
    {0xC0, 7, MODE_8, FLG_RM, FLG_IMM8, OPC_SAR},

    {0xC1, 0, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_ROL},
    {0xC1, 1, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_ROR},
    {0xC1, 2, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_RCL},
    {0xC1, 3, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_RCR},
    {0xC1, 4, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_SHL},
    {0xC1, 5, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_SHR},
    {0xC1, 6, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_SAL},
    {0xC1, 7, MODE_DEFAULT, FLG_RM, FLG_IMM8, OPC_SAR},

    {0xC6, 0, MODE_8, FLG_RM, FLG_IMM8, OPC_MOV},
    {0xC7, 0, MODE_DEFAULT, FLG_RM, FLG_IMM, OPC_MOV},

    {0xD0, 0, MODE_8, FLG_RM, FLG_CONST_1, OPC_ROL},
    {0xD0, 1, MODE_8, FLG_RM, FLG_CONST_1, OPC_ROR},
    {0xD0, 2, MODE_8, FLG_RM, FLG_CONST_1, OPC_RCL},
    {0xD0, 3, MODE_8, FLG_RM, FLG_CONST_1, OPC_RCR},
    {0xD0, 4, MODE_8, FLG_RM, FLG_CONST_1, OPC_SHL},
    {0xD0, 5, MODE_8, FLG_RM, FLG_CONST_1, OPC_SHR},
    {0xD0, 6, MODE_8, FLG_RM, FLG_CONST_1, OPC_SAL},
    {0xD0, 7, MODE_8, FLG_RM, FLG_CONST_1, OPC_SAR},

    {0xD1, 0, MODE_DEFAULT, FLG_RM, FLG_CONST_1, OPC_ROL},
    {0xD1, 1, MODE_DEFAULT, FLG_RM, FLG_CONST_1, OPC_ROR},
    {0xD1, 2, MODE_DEFAULT, FLG_RM, FLG_CONST_1, OPC_RCL},
    {0xD1, 3, MODE_DEFAULT, FLG_RM, FLG_CONST_1, OPC_RCR},
    {0xD1, 4, MODE_DEFAULT, FLG_RM, FLG_CONST_1, OPC_SHL},
    {0xD1, 5, MODE_DEFAULT, FLG_RM, FLG_CONST_1, OPC_SHR},
    {0xD1, 6, MODE_DEFAULT, FLG_RM, FLG_CONST_1, OPC_SAL},
    {0xD1, 7, MODE_DEFAULT, FLG_RM, FLG_CONST_1, OPC_SAR},

    {0xD2, 0, MODE_8, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_ROL},
    {0xD2, 1, MODE_8, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_ROR},
    {0xD2, 2, MODE_8, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_RCL},
    {0xD2, 3, MODE_8, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_RCR},
    {0xD2, 4, MODE_8, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_SHL},
    {0xD2, 5, MODE_8, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_SHR},
    {0xD2, 6, MODE_8, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_SAL},
    {0xD2, 7, MODE_8, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_SAR},

    {0xD3, 0, MODE_DEFAULT, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_ROL},
    {0xD3, 1, MODE_DEFAULT, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_ROR},
    {0xD3, 2, MODE_DEFAULT, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_RCL},
    {0xD3, 3, MODE_DEFAULT, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_RCR},
    {0xD3, 4, MODE_DEFAULT, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_SHL},
    {0xD3, 5, MODE_DEFAULT, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_SHR},
    {0xD3, 6, MODE_DEFAULT, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_SAL},
    {0xD3, 7, MODE_DEFAULT, FLG_RM, FLG_REG_CONST | CL | FLG_8BIT, OPC_SAR},

    {0xF6, 0, MODE_8, FLG_RM, FLG_IMM8, OPC_TEST},
    {0xF6, 1, MODE_8, FLG_RM, FLG_IMM8, OPC_TEST},
    {0xF6, 2, MODE_8, FLG_RM, FLG_NONE, OPC_NOT},
    {0xF6, 3, MODE_8, FLG_RM, FLG_NONE, OPC_NEG},
    {0xF6, 4, MODE_8, FLG_RM, FLG_NONE, OPC_MUL},
    {0xF6, 5, MODE_8, FLG_RM, FLG_NONE, OPC_IMUL},
    {0xF6, 6, MODE_8, FLG_RM, FLG_NONE, OPC_DIV},
    {0xF6, 7, MODE_8, FLG_RM, FLG_NONE, OPC_IDIV},

    {0xF7, 0, MODE_DEFAULT, FLG_RM, FLG_IMM, OPC_TEST},
    {0xF7, 1, MODE_DEFAULT, FLG_RM, FLG_IMM, OPC_TEST},
    {0xF7, 2, MODE_DEFAULT, FLG_RM, FLG_NONE, OPC_NOT},
    {0xF7, 3, MODE_DEFAULT, FLG_RM, FLG_NONE, OPC_NEG},
    {0xF7, 4, MODE_DEFAULT, FLG_RM, FLG_NONE, OPC_MUL},
    {0xF7, 5, MODE_DEFAULT, FLG_RM, FLG_NONE, OPC_IMUL},
    {0xF7, 6, MODE_DEFAULT, FLG_RM, FLG_NONE, OPC_DIV},
    {0xF7, 7, MODE_DEFAULT, FLG_RM, FLG_NONE, OPC_IDIV},

    {0xFE, 0, MODE_8, FLG_RM, FLG_NONE, OPC_INC},
    {0xFE, 1, MODE_8, FLG_RM, FLG_NONE, OPC_DEC},

    {0xFF, 0, MODE_DEFAULT, FLG_RM, FLG_NONE, OPC_INC},
    {0xFF, 1, MODE_DEFAULT, FLG_RM, FLG_NONE, OPC_DEC},

    /** IFFY ON 3 AND 5 (CALLF AND JMP) */
    {0xFF, 2, MODE_DEFAULT, FLG_RM, FLG_NONE, OPC_CALL},
    {0xFF, 3, MODE_DEFAULT, FLG_PTR16, FLG_NONE, OPC_CALL},
    {0xFF, 4, MODE_DEFAULT, FLG_RM, FLG_NONE, OPC_JMP},
    {0xFF, 5, MODE_DEFAULT, FLG_PTR16, FLG_NONE, OPC_CALL},

    /* for some reason (google isn't available with no internet), I can't use sizeof() on an
     * extern struct [] of this type so I'm marking the end of the array with a bunch of nuls*/
    {0,0,0,0,0,0}
};













