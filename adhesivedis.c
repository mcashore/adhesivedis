#include <stdio.h>
#include <string.h>
#include "opcode.h"
#include "instruction.h"
#include "readopcode.h"

#define IS_INVALID(x) ( (x) == FLG_INVALID || (x) & FLG_PREFIX )

void read_past_prefix(struct x86_instruction *instr, unsigned char **src)
{
	int group;
	memset(instr->prefix, 0, sizeof(instr->prefix));

	while(1)
	{
		switch(**src)
		{
			/* rep / repne / lock prefixes */
			case 0xF0:
			case 0xF2:
			case 0xF3:
				group = 1;
				break;

			/* segment-override prefixes */
			case 0x2E: case 0x36:
			case 0x3E: case 0x26:
			case 0x64: case 0x65:
				group = 2;
				break;

			/* operand size-override */
			case 0x66:
				group = 3;
				break;

			/* address size-override */
			case 0x67:
				group = 4;
				break;

			default:
				return;
		}

		/* there can only be one prefix from each group! */
		if(instr->prefix[group-1] != 0)
			return;

		instr->prefix[group-1] = *(*src)++;
	}
}


/**
 * if the operand is specified by the rm member of the modrm byte,
 * this function does the dirty work
 */
int parse_rm(struct operand *operand, struct modrm *modrm, enum mode address_mode)
{
    DPRINT("modrm->mod = %d, modrm->reg = %d, modrm->rm = %d\n", modrm->mod, modrm->reg, modrm->rm);

    operand->reg = modrm->rm;

    /* is the operand rm itself?! */
    if(modrm->mod == 0x03)
    {
        DPRINT("the operand is modrm->rm itself! %02X\n", modrm->rm);
        return 1;
    }

    /* the intel manuals say a mod of 00b and an rm of 101b specifies an address_mode displacement...
    but there's no register associated with it so it confuses me so this is here instead */
    if(modrm->mod == 0 && modrm->rm == 5)
    {
        DPRINT("NOT IMPLEMENTED YET: 32 bit displacement without register..?\n");
        return 0;
    }

    /* if mod isn't 11b it's a pointer */
    operand->flags |= HAS_POINTER;
    DPRINT("modrm->rm is a pointer\n");

    /* check for displacement */
    if(modrm->mod == 0x01)
    {
        operand->flags |= HAS_DISPLACEMENT;
        operand->disp_mode = MODE_8;
        DPRINT("8-bit displacement present\n");
    }
    else if(modrm->mod == 0x02)
    {
        operand->flags |= HAS_DISPLACEMENT;
        operand->disp_mode = address_mode;
        DPRINT("%d-bit displacement present\n", address_mode * 8);
    }

    /* do we have an sib? :o*/
    if(address_mode == MODE_32 && modrm->rm == 0x04)
    {
        DPRINT("sib byte present!\n");
        operand->flags |= HAS_SIB;
        return 1;
    }


    return 1;
}

/**
 * only implemented opcodes will reach this point so there's no need to make sure they're valid
 */
int parse_operands(struct opcode *opcode, struct x86_instruction *instr, unsigned char **src)
{
    struct operand *operand = instr->operands;
    int i;

    /* a bool to check if the modr/m's been read yet and a byte in which to store the modrm */
    unsigned char read_modrm = 0;
    struct modrm *modrm;


    if(opcode->operands[0] & FLG_8BIT)
        instr->operand_mode = MODE_8;

    DPRINT("\n\n");

    for(i = 0; i < MAX_OPERAND; i++)
    {
        /* if the operand's not used we're done */
        if(opcode->operands[i] == FLG_NONE)
        {
            DPRINT("operand %d isn't used\n", i + 1);
            break;
        }

        /* check if we need to read the ModR/M byte */
        if(opcode->operands[i] & FLG_REG || opcode->operands[i] & FLG_RM || opcode->operands[i] & FLG_SREG)
        {
            if(!read_modrm)
            {
                read_modrm = 1;
                modrm = (void *)(*src)++;
                DPRINT("mod = %02X, reg = %02X, rm = %02X\n", modrm->mod, modrm->reg, modrm->rm);
            }
        }

        /* There's a register embedded in the opcode (and in the flags) */
        if(opcode->operands[i] & FLG_REG_CONST)
        {
            operand->flags |= HAS_REGISTER;
            /* the three least significant bits specify the reg in this case*/
            operand->reg = opcode->operands[i] & 0x07;
            DPRINT("constant register = %02X\n", operand->reg);
        }

        /* There's a segment register embedded in the opcode */
        else if(opcode->operands[i] & FLG_SEG_CONST)
        {
            operand->flags |= HAS_SEGMENT;
            operand->reg = opcode->operands[i] & 0x07;
            DPRINT("constant segment = %02X\n", operand->reg);
        }

        /* The reg field of the ModR/M byte specifies the register used */
        else if(opcode->operands[i] & FLG_REG)
        {
            operand->flags |= HAS_REGISTER;
            operand->reg = modrm->reg;
            DPRINT("register specified by modrm->reg = %02X\n", modrm->reg);
        }

        /* The reg field of the ModR/M byte specifies a segment register */
        else if(opcode->operands[i] & FLG_SREG)
        {
            operand->flags |= HAS_SEGMENT;
            operand->reg = modrm->reg;
            DPRINT("segment register specified by modrm->reg = %02X\n", modrm->reg);
        }

        /* the R/M portion of the ModR/M byte specifies this operand*/
        else if(opcode->operands[i] & FLG_RM)
        {

            operand->flags |= HAS_REGISTER;
            if(!parse_rm(operand, modrm, instr->address_mode))
            {
                DPRINT("modrm parsing failed...\n");
                return 0;
            }
            else if(!(operand->flags & HAS_POINTER) && opcode->operands[i] & FLG_MODRM_MEM)
            {
                DPRINT("modrm doesn't specify memory when it's supposed to!\n");
                return 0;
            }
            else
                DPRINT("parsed modrm successfully\n");
        }

        /* There is an 8 bit immediate value */
        else if(opcode->operands[i] & FLG_IMM8)
        {
            operand->flags |= HAS_IMMEDIATE;
            operand->imm_mode = MODE_8;
            DPRINT("we have an 8-bit immediate\n");
        }

        /* There is a 16 bit immediate value */
        else if(opcode->operands[i] & FLG_IMM16)
        {
            operand->flags |= HAS_IMMEDIATE;
            operand->imm_mode = MODE_16;
            DPRINT("we have a 16-bit immediate\n");
        }

        /* There is an instr->operand_mode bit immediate value */
        else if(opcode->operands[i] & FLG_IMM)
        {
            operand->flags |= HAS_IMMEDIATE;
            operand->imm_mode = instr->operand_mode;
            DPRINT("we have a %d-bit immediate\n", instr->operand_mode * 8);
        }

        /* The immediate value is an 8 bit relative offset */
        else if(opcode->operands[i] & FLG_REL8)
        {
            operand->flags |= HAS_REL_IMM;
            operand->imm_mode = MODE_8;
            DPRINT("we have an 8-bit relative immediate\n");
        }

        /* The immediate value is an instr->operand_mode relative offset */
        else if(opcode->operands[i] & FLG_REL)
        {
            operand->flags |= HAS_REL_IMM;
            operand->imm_mode = instr->operand_mode;
            DPRINT("we have a %d-bit relative immediate\n", instr->operand_mode * 8);
        }

        else if(opcode->operands[i] & FLG_PTR16)
        {
            DPRINT("operand %d is a far pointer - not implemented yet\n", i);
            return 0;
        }
         /* FIXME: nasty hack (again) (also this has to be at the end so it doesn't conflict with constant registers)
         * really need to fix this */
        else if(opcode->operands[i] & FLG_CONST_1)
        {
            operand->flags |= HAS_CONST_1;
            continue;
        }


        if(operand->flags & HAS_REGISTER)
        {
            if(operand->flags & HAS_POINTER)
                operand->reg_mode = instr->address_mode;
            else if( opcode->operands[i] & FLG_8BIT)
                operand->reg_mode = MODE_8;
            else
                operand->reg_mode = instr->operand_mode;
        }

        DPRINT("done parsing operand %d!\n\n\n", i + 1);
        operand++;
    }
    DPRINT("\n\n\n");
    instr->number_of_operands = i;

    return 1;
}

/**
 * Grabs stuff like displacement and immediate values from the instruction
 * assumes src points to the byte after the opcode
 */
int grab_values(struct x86_instruction *instr, unsigned char **src)
{
    struct operand *operand = instr->operands;
    int i = instr->number_of_operands;

    /* TODO: make sure it's a valid instruction - i.e. immediate comes after displacment
                                                  i.e.e. mov 5,[eax+4] is invalid
    */
    while(i--)
    {
        /* FIXME: this is a stupid way of handling this and it should be fixed */
        if(operand->flags & HAS_CONST_1)
        {
            operand->flags |= HAS_IMMEDIATE;
            operand->imm_mode = MODE_8;
            operand->imm.imm8 = 1;
            continue;
        }


        if(operand->flags & HAS_SIB)
        {
            operand->sib.scale = **src >> 6;
            operand->sib.index = (**src >> 3) & 7;
            operand->sib.base = **src & 7;
            (*src)++;
        }

        if(operand->flags & HAS_DISPLACEMENT)
        {
            DPRINT("**src = %02X\n", **src);
            switch(operand->disp_mode)
            {
                case MODE_8:
                    operand->disp.disp8 = *(*src)++;
                    break;
                case MODE_16:
                    operand->disp.disp16 = *(unsigned short *)(*src);
                    *src += 2;
                    break;
                case MODE_32:
                    operand->disp.disp32 = *(unsigned long *)(*src);
                    *src += 4;
                    break;
            }
        }

        if(operand->flags & HAS_IMMEDIATE || operand->flags & HAS_REL_IMM)
        {
            switch(operand->imm_mode)
            {
                case MODE_8:
                    operand->imm.imm8 = *(*src)++;
                    break;
                case MODE_16:
                    operand->imm.imm16 = *(unsigned short *)(*src);
                    *src += 2;
                    break;
                case MODE_32:
                    operand->imm.imm32 = *(unsigned long *)(*src);
                    *src += 4;
                    break;
            }
        }

        operand++;
    }
    /* does no error checking, bad bad bad */
    return 1;
}

int parse_instruction(struct x86_instruction *instr, unsigned char *src)
{
	struct opcode opcode;
	unsigned char *src_ = src;

	memset(instr, 0, sizeof(*instr));

	/* read prefixes */
	read_past_prefix(instr, &src);

    /* reand the opcode and make sure it's valid */
    if(!read_opcode(instr, &opcode, &src))
        return 0;

    /* handle operand size override */
    if(opcode.operand_size == MODE_DEFAULT && instr->prefix[PREF_OP])
        opcode.operand_size = (opcode.operand_size == MODE_32) ? MODE_16 : MODE_32;
    instr->operand_mode = opcode.operand_size;

    /* handle address size override */
    if(instr->prefix[PREF_ADDR])
        instr->address_mode = (MODE_DEFAULT == MODE_32) ? MODE_16 : MODE_32;
    else
        instr->address_mode = MODE_DEFAULT;

    /* finish parsing and make sure everythign goes well */
    if(!parse_operands(&opcode, instr, &src))
        return 0;

    if(!grab_values(instr, &src))
        return 0;

    instr->length = src - src_;

    /* success!*/
	return 1;
}




