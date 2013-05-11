#include <string.h>
#include "opcode.h"
#include "instruction.h"


extern struct opcode_one_byte one_byte_opcodes[];
extern struct opcode_group_one_byte one_byte_opcode_groups[];

#define sizeofarr(arr) (sizeof(arr) / sizeof((arr)[0]))
#define IS_INVALID(x) ( (x) == FLG_INVALID || (x) & FLG_PREFIX )

static int read_one_byte(struct x86_instruction *instr, struct opcode *opcode, unsigned char **src)
{
    if(IS_INVALID(one_byte_opcodes[**src].operand1))
        return 0;

    opcode->operands[0] = one_byte_opcodes[**src].operand1;
    opcode->operands[1] = one_byte_opcodes[**src].operand2;
    DPRINT("operands[0] = %08X\noperands[1] = %08X\n", opcode->operands[0], opcode->operands[1]);
    opcode->operand_size = one_byte_opcodes[**src].mode;

    opcode->opcode_len = 1;
    instr->opcode_len = 1;



    instr->opcode = opcode->opcode = one_byte_opcodes[**src].opcode;

    opcode->opcode_bytes[0] = *(*src)++;
    instr->opcode_bytes[0] = opcode->opcode_bytes[0];

    DPRINT("**src = %d  opcode = %d\n", one_byte_opcodes[**src].opcode);
    return 1;
}

static int read_one_byte_group(struct x86_instruction *instr, struct opcode *opcode, unsigned char **src)
{
    unsigned char reg = (*(*src + 1) >> 3) & 7, i;
    DPRINT("reading one byte opcode from group where byte = %02X and reg = %02X and rm = %02X\n", **src, reg, *(*src + 1) & 7);

    for(i = 0; one_byte_opcode_groups[i].byte; i++)
    {
        if(one_byte_opcode_groups[i].byte == **src && one_byte_opcode_groups[i].reg == reg)
        {
            opcode->operands[0] = one_byte_opcode_groups[i].operand1;
            opcode->operands[1] = one_byte_opcode_groups[i].operand2;
            opcode->operand_size = one_byte_opcode_groups[i].mode;
            opcode->opcode_len = 1;
            opcode->opcode_bytes[0] = *(*src)++;
            instr->opcode_len = 1;
            instr->opcode_bytes[0] = opcode->opcode_bytes[0];
            instr->opcode = opcode->opcode = one_byte_opcode_groups[i].opcode;
            return 1;
        }
    }
    return 0;
}

int read_opcode(struct x86_instruction *instr, struct opcode *opcode, unsigned char **src)
{
    /* only supports one byte opcodes atm */
    DPRINT("Reading opcode with byte %02X\n", **src);
    memset(opcode, 0, sizeof(*opcode));

    switch(one_byte_opcodes[**src].table)
    {
        case TBL_ONE_BYTE:
            return read_one_byte(instr, opcode, src);

        case TBL_ONE_BYTE_GRP:
            return read_one_byte_group(instr, opcode, src);

        case TBL_NONE:
        default:
            return 0;
    }

}
