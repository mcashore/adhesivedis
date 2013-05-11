#include <string.h>
#include <stdio.h>
#include "opcode.h"
#include "instruction.h"

static const char *reg32[] = {
    "EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"
};

static const char *reg16[] = {
    "AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"
};

static const char *reg8[] = {
    "AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"
};

static const char *segments[] = {
    "CS", "SS", "DS", "ES", "FS", "GS"
};

static const char *opcodes[] = {
    "ADD",
    "PUSH",
    "POP",
    "OR",
    "ADC",
    "SBB",
    "AND",
    "DAA",
    "SUB",
    "DAS",
    "XOR",
    "AAA",
    "CMP",
    "AAS",
    "INC",
    "DEC",
    "PUSHA",
    "POPA",
    "BOUND",
    "ARPL",
    "IMUL",
    "INS",
    "OUTS",
    "JO",
    "JNO",
    "JB",
    "JNB",
    "JE",
    "JNE",
    "JBE",
    "JNBE",
    "JS",
    "JNS",
    "JP",
    "JNP",
    "JL",
    "JNL",
    "JLE",
    "JNLE",
    "TEST",
    "XCHG",
    "MOV",
    "LEA",
    "NOP",
    "CBW",
    "CDQ",
    "CALL",
    "WAIT",
    "PUSHF",
    "PUSHFD",
    "POPF",
    "POPFD",
    "SAHF",
    "LAHF",
    "MOVS",
    "CMPS",
    "STOS",
    "LODS",
    "SCAS",
    "ROL",
    "ROR",
    "RCL",
    "RCR",
    "SHL",
    "SHR",
    "SAL",
    "SAR",
    "RETN",
    "LES",
    "LDS",
    "ENTER",
    "LEAVE",
    "RETF",
    "INT3",
    "INT",
    "INTO",
    "IRET",
    "SALC",
    "XLAT",
    "LOOPNZ",
    "LOOPZ",
    "LOOP",
    "JECXZ",
    "IN",
    "OUT",
    "JMP",
    "JMPF",
    "HLT",
    "CMC",
    "NOT",
    "CLC",
    "STC",
    "CLI",
    "STI",
    "CLD",
    "STD",
    "NEG",
    "MUL",
    "DIV",
    "IDIV"
};



static int append(char *str, const char *apnd, size_t *length_left)
{
    size_t len = strlen(apnd);
    if(len > *length_left)
        return 0;
    else
    {
        *length_left -= len;
        strcat(str, apnd);
        return 1;
    }
}

static inline int append_byte(char *str, unsigned char byte, size_t *length_left)
{
    char tempbuf[16];
    sprintf(tempbuf, "0x%02lX", (unsigned long)byte);
    return append(str, tempbuf, length_left);
}

static inline int append_word(char *str, unsigned short word, size_t *length_left)
{
    char tempbuf[16];
    sprintf(tempbuf, "0x%04lX", (unsigned long)word);
    return append(str, tempbuf, length_left);
}

static inline int append_dword(char *str, unsigned long dword, size_t *length_left)
{
    char tempbuf[16];
    sprintf(tempbuf, "0x%08lX", dword);
    return append(str, tempbuf, length_left);
}

int append_operand(struct operand *operand, char *buf, size_t *len)
{
    const char **temp;
    if(operand->flags & HAS_POINTER)
    {
        if(!append(buf, "[", len))
            return 0;
    }

    if(operand->flags & HAS_SIB)
    {
        if(operand->reg_mode == MODE_16)
            temp = reg16;
        else if(operand->reg_mode == MODE_32)
            temp = reg32;
        else
            return 0;

        if(!append(buf, temp[operand->sib.index], len))
            return 0;

        switch(operand->sib.scale)
        {
            case 0: break;
            case 1:
                if(!append(buf, "*2", len))
                    return 0;
                break;
            case 2:
                if(!append(buf, "*4", len))
                    return 0;
                break;
            case 3:
                if(!append(buf, "*8", len))
                    return 0;
                break;
            default:
                return 0;
        }
        if(!append(buf, "+", len))
            return 0;
        if(!append(buf, temp[operand->sib.base], len))
            return 0;
    }
    else if(operand->flags & HAS_REGISTER)
    {
        switch(operand->reg_mode)
        {
            case MODE_8:
                if(!append(buf, reg8[operand->reg], len))
                    return 0;
                break;
            case MODE_16:
                if(!append(buf, reg16[operand->reg], len))
                    return 0;
                break;
            case MODE_32:
                if(!append(buf, reg32[operand->reg], len))
                    return 0;
                break;
        }
    }
    else if(operand->flags & HAS_SEGMENT)
    {
        if(!append(buf, segments[operand->reg], len))
            return 0;
    }
    else if(operand->flags & HAS_IMMEDIATE || operand->flags & HAS_REL_IMM)
    {
        switch(operand->imm_mode)
        {
            case MODE_8:
                if(!append_byte(buf, operand->imm.imm8, len))
                    return 0;
                break;
            case MODE_16:
                if(!append_word(buf, operand->imm.imm16, len))
                    return 0;
                break;
            case MODE_32:
                if(!append_dword(buf, operand->imm.imm32, len))
                    return 0;
                break;
        }
    }

    if(operand->flags & HAS_DISPLACEMENT)
    {
        if(!append(buf, " + ", len))
            return 0;
        switch(operand->disp_mode)
        {
            case MODE_8:
                if(!append_byte(buf, operand->disp.disp8, len))
                    return 0;
                break;
            case MODE_16:
                if(!append_word(buf, operand->disp.disp16, len))
                    return 0;
                break;
            case MODE_32:
                if(!append_dword(buf, operand->disp.disp32, len))
                    return 0;
                break;
        }
    }


    if(operand->flags & HAS_POINTER)
    {
        if(!append(buf, "]", len))
            return 0;
    }
    return 1;
}

int build_string(struct x86_instruction *instr, char *buf, size_t len)
{
    unsigned int i;

    memset(buf, 0, len);

    if(!append(buf, opcodes[instr->opcode], &len))
        return 0;

    if(!append(buf, " ", &len))
        return 0;

    for(i = 0; i < instr->number_of_operands; i++)
    {
        if(!append_operand(&instr->operands[i], buf, &len))
            return 0;

        if(i < instr->number_of_operands - 1)
        {
            if(!append(buf, ", ", &len))
                return 0;
        }
    }

    return 1;
}






