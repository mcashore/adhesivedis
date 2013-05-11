#ifndef OPCODE_H_INCLUDED
#define OPCODE_H_INCLUDED

//#define DEBUG

#ifdef DEBUG
# include <stdio.h>
# define DPRINT(fmt, arg...) do { printf(fmt, ##arg); fflush(stdout); } while(0)
#else
# define DPRINT(fmt, arg...)
#endif

#define MODE_INVALID -1
enum mode
{
    MODE_8 = 1,
    MODE_16 = 2,
    MODE_32 = 4
};

#define MODE_DEFAULT MODE_32

// registers & stuff
enum segments {CS,SS,DS,ES,FS,GS};
enum registers {EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI};
enum registers8 {AL,CL,DL,BL,AH,CH,DH,BH};

// opcodes
enum opcodes
{
    OPC_ADD,
    OPC_PUSH,
    OPC_POP,
    OPC_OR,
    OPC_ADC,
    OPC_SBB,
    OPC_AND,
    OPC_DAA,
    OPC_SUB,
    OPC_DAS,
    OPC_XOR,
    OPC_AAA,
    OPC_CMP,
    OPC_AAS,
    OPC_INC,
    OPC_DEC,
    OPC_PUSHA,
    OPC_POPA,
    OPC_BOUND,
    OPC_ARPL,
    OPC_IMUL,
    OPC_INS,
    OPC_OUTS,
    OPC_JO,
    OPC_JNO,
    OPC_JB,
    OPC_JNB,
    OPC_JE,
    OPC_JNE,
    OPC_JBE,
    OPC_JNBE,
    OPC_JS,
    OPC_JNS,
    OPC_JP,
    OPC_JNP,
    OPC_JL,
    OPC_JNL,
    OPC_JLE,
    OPC_JNLE,
    OPC_TEST,
    OPC_XCHG,
    OPC_MOV,
    OPC_LEA,
    OPC_NOP,
    OPC_CBW,
    OPC_CDQ,
    OPC_CALL,
    OPC_WAIT,
    OPC_PUSHF,
    OPC_PUSHFD,
    OPC_POPF,
    OPC_POPFD,
    OPC_SAHF,
    OPC_LAHF,
    OPC_MOVS,
    OPC_CMPS,
    OPC_STOS,
    OPC_LODS,
    OPC_SCAS,
    OPC_ROL,
    OPC_ROR,
    OPC_RCL,
    OPC_RCR,
    OPC_SHL,
    OPC_SHR,
    OPC_SAL,
    OPC_SAR,
    OPC_RETN,
    OPC_LES,
    OPC_LDS,
    OPC_ENTER,
    OPC_LEAVE,
    OPC_RETF,
    OPC_INT3,
    OPC_INT,
    OPC_INTO,
    OPC_IRET,
    OPC_SALC,
    OPC_XLAT,
    OPC_LOOPNZ,
    OPC_LOOPZ,
    OPC_LOOP,
    OPC_JECXZ,
    OPC_IN,
    OPC_OUT,
    OPC_JMP,
    OPC_JMPF,
    OPC_HLT,
    OPC_CMC,
    OPC_NOT,
    OPC_CLC,
    OPC_STC,
    OPC_CLI,
    OPC_STI,
    OPC_CLD,
    OPC_STD,
    OPC_NEG,
    OPC_MUL,
    OPC_DIV,
    OPC_IDIV
} ;

// flags
#define FLG_NONE 0
#define FLG_INVALID 0xFFFF
#define FLG_NOT_IMPLEMENTED FLG_INVALID



enum opcode_flags
{
    FLG_IMM     = (1 << 4), /* 16, or 32 bit immediate value */
    FLG_REG     = (1 << 16), /* the 'reg' field in the modR/M byte specifies this op */
    FLG_8BIT    = (1 << 5), /* the operand is 8 bits (if this is set for one operand in the instruction it is assumed for all) */
    FLG_PREFIX  = (1 << 6), /* this byte specifies a prefix */
    FLG_SEG_CONST = (1 << 7), /* there is a constant segment register */
    FLG_REG_CONST = (1 << 8), /* this operation modifies a constant register (the lower three bits of this flag) */
    FLG_RM   = (1 << 9),  /* the 'r/m' field in the modR/M byte specifies this op */
    FLG_REL     = (1 << 10), /* 16 or 32 bit relative offset */
    FLG_IMM8    = (1 << 11), /* 8 bit immediate value */
    FLG_REL8    = (1 << 12), /* 8 relative offset */
    FLG_SREG    = (1 << 13), /* The reg field of the ModR/M byte selects a segment register */
    FLG_MODRM_MEM = (1 << 14), /* the ModR/M byte may only refer to memory (mod != 11b) */
    FLG_PTR16   = (1 << 15),
    FLG_IMM16   = (1 << 3),

    FLG_CONST_1 = (1 << 17) /* the operand is a constant value of 1 FIXME: dirty hack */
};

enum prefix
{
    PREF_REP = 0,
    PREF_SEG = 1,
    PREF_OP = 2,
    PREF_ADDR = 3
};

/* A list of the different opcode tables */
enum tables
{
    TBL_NONE,
    TBL_ONE_BYTE, /* */
    TBL_ONE_BYTE_GRP,
};

/* specifies a one byte opcode without an extension or a secondary opcode or a third operand */
struct opcode_one_byte
{
    int table; /* if this isn't a valid instr, what table do we use instead? */
    int mode; /* default operand mode */

    unsigned long operand1; /* flags */
    unsigned long operand2; /* flags */

	int opcode; /* constant for mnemonics */
} __attribute__((packed));


struct opcode_group_one_byte
{
    unsigned char byte; /* the byte of the main opcode */
    unsigned char reg;  /* what must modrm->reg be ? */

    /* enum mode isn't used because I can't apply sizeof() to an enum :( */
    int mode; /* default operand mode */

    unsigned long operand1; /* flags */
    unsigned long operand2; /* flags */

    /* enum opcodes isn't used for the same reason as above */
	int opcode; /* constant for mnemonics */
} __attribute__((packed));

#ifndef MAX_OPCODE
#define MAX_OPCODE 3
#endif

#ifndef MAX_OPERAND
#define MAX_OPERAND 2
#endif

struct opcode
{
    enum mode operand_size;

    unsigned char opcode_len;
    unsigned char opcode_bytes[MAX_OPCODE];

    unsigned char number_of_operands;
    unsigned long operands[MAX_OPERAND];

    int opcode;
};






#endif // OPCODE_H_INCLUDED













