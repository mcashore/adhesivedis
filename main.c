#include <stdio.h>
#include "adhesivedis.h"

#if 1
#include <string.h>
#include <stdlib.h>

void disas_code(unsigned char *code, size_t len)
{
    struct x86_instruction instr;
    char str[128] = {0};
    int i;
    unsigned char *lim = code + len;
    while(code < lim)
    {
        if(!parse_instruction(&instr, code))
        {
            printf("parsing failed\n");
            return;
        }
        if(build_string(&instr, str, sizeof(str) - 1) <= 0)
        {
            printf("turning into a string failed\n");
            return;
        }

        for(i = 0; i < instr.length; i++)
            printf("%02X", code[i]);
        printf("\t(%d) %s\n", instr.length, str);
        code += instr.length;

    }
}


void disas_bin_file(char *filename)
{
    FILE *bin = fopen(filename, "rb");
    unsigned char *buf;
    size_t size;
    if(!bin)
    {
        fprintf(stderr, "couldn't open %s\n", filename);
        return;
    }

    fseek(bin, 0, SEEK_END);
    size = ftell(bin);
    fseek(bin, 0, SEEK_SET);

    buf = malloc(size * sizeof(*buf));
    if(!buf)
    {
        fprintf(stderr, "couldn't allocate memory\n");
        return;
    }

    if(fread(buf, sizeof(*buf), size, bin) != size)
    {
        fprintf(stderr, "couldn't read whole file\n");
        return;
    }

    disas_code(buf, size);

    free(buf);
    fclose(bin);

}



#define FILENAME "C:\\Users\\Massey\\rc4"
int main(void)
{
    disas_bin_file(FILENAME);
    return 0;
}

#else


#define INSTRUCTION "\x81\xF9\xFF\x00\x00\x00"

int main(int argc, char *argv[])
{
    char str[128] = {0};
    struct x86_instruction instr;
    unsigned char *src;


    if(argc < 2)
        src = (unsigned char *)INSTRUCTION;
    else
        src = (unsigned char *)argv[1];

    if(parse_instruction(&instr, src))
    {
        print_instruction(&instr);
        if(build_string(&instr, str, sizeof(str)- 1))
        {
            printf("built string!\n");
            printf("%s\n", str);
        }
        else
        {
            printf("error building string\n");
        }
    }
    else
        puts("error");

    return 0;
}
#endif
