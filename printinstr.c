#include <stdio.h>
#include "opcode.h"
#include "instruction.h"

/**
 * Debug purposes, this whole file sucks really
 */


void print_operand(struct operand *operand)
{
    if(operand->flags & HAS_POINTER)
        printf("\toperand is a pointer\n\n");

    if(operand->flags & HAS_REGISTER)
        printf("\toperand has a register.  reg = %d, mode = %d\n\n", operand->reg, operand->reg_mode * 8);
    else if(operand->flags & HAS_SEGMENT)
        printf("\toperand has a segment register.  seg = %d\n\n", operand->reg);

    if(operand->flags & HAS_SIB)
    {
        printf("sib byte present\n");
        printf("\tsib.scale = %d, sib.index = %d, sib.base = %d\n\n",
               operand->sib.scale, operand->sib.index, operand->sib.base);
    }

    if(operand->flags & HAS_DISPLACEMENT)
    {
        printf("displacement present\n");
        switch(operand->disp_mode)
        {
            case MODE_8:
                printf("\tdisp.disp8 = %02X\n\n", operand->disp.disp8);
                break;
            case MODE_16:
                printf("\tdisp.disp16 = %04X\n\n", operand->disp.disp16);
                break;
            case MODE_32:
                printf("\tdisp.disp32 = %08lX\n\n", operand->disp.disp32);
                break;
        }
    }

    if(operand->flags & HAS_IMMEDIATE)
        printf("immediate value present\n");
    else if(operand->flags & HAS_REL_IMM)
        printf("relative immediate value present\n");
    else
        return;

    switch(operand->imm_mode)
    {
        case MODE_8:
            printf("\timm.imm8 = %02X\n\n", operand->imm.imm8);
            break;
        case MODE_16:
            printf("\timm.imm16 = %04X\n\n", operand->imm.imm16);
            break;
        case MODE_32:
            printf("\timm.imm32 = %08lX\n\n", operand->imm.imm32);
            break;
    }

}

void print_instruction(struct x86_instruction *instr)
{
    int i;
    printf("\n\n\n----- dumping information on instruction -----\n\n\n");

    printf("prefixes:\n");
	if(instr->prefix[0]) printf("\tgroup 1: %02X\n", instr->prefix[0]);
	if(instr->prefix[1]) printf("\tgroup 2: %02X\n", instr->prefix[1]);
	if(instr->prefix[2]) printf("\tgroup 3: %02X\n", instr->prefix[2]);
	if(instr->prefix[3]) printf("\tgroup 4: %02X\n", instr->prefix[3]);

    printf("\nopcode:\n");
    printf("\t%d byte opcode: ", instr->opcode_len);
    for(i = 0; i < instr->opcode_len; i++)
        printf("%02X", instr->opcode_bytes[i]);
    putchar('\n');

    printf("\noperand mode:\n");
    if(instr->operand_mode == MODE_8)
        printf("\t8 bits\n");
    else if(instr->operand_mode == MODE_16)
        printf("\t16 bits\n");
    else if(instr->operand_mode == MODE_32)
        printf("\t32 bits\n");

    printf("\naddress mode:\n");
    if(instr->address_mode == MODE_8)
        printf("\t8 bits\n");
    else if(instr->address_mode == MODE_16)
        printf("\t16 bits\n");
    else if(instr->address_mode == MODE_32)
        printf("\t32 bits\n");

    printf("\ninstruction len = %02X\n", instr->length);
    for(i = 0; i < instr->number_of_operands; i++)
    {
        printf("\n\n--- dumping information on operand %d ---\n\n", i);
        print_operand(&instr->operands[i]);
    }
}
