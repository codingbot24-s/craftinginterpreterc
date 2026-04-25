
#include <stdio.h>
#include "debug.h"
#include "value.h"


int simpleInstruction(const char* name,int offset)
{
    printf("%s\n", name);
    return offset + 1;
} 

int constant_instruction(const char* name,Chunk* c, int offset) 
{
    uint8_t c_index = c->code[offset + 1];
    // no new line buffered
    printf("%-16s %4d '", name, c_index);

    print_value(c->constants.Values[c_index]);
    printf("'\n");
    return offset + 2;
}

int disassembleInstruction(Chunk* c, int offset) 
{
    printf("%04d ", offset);
    if (offset  > 0 && c->line[offset] == c->line[offset - 1])
    {
        printf("   | ");
    }else 
    {
        printf("%4d ", c->line[offset]);
    }
    
    uint8_t instruction = c->code[offset];
    switch (instruction)
    {
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    case OP_CONSTANT:
        return constant_instruction("OP_CONSTANT",c,offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}



void disassembleChunk(Chunk* c,const char* name)
{
    printf("== %s ==\n", name);
   
    for (int offset = 0; offset < c->count;)
    {
        offset = disassembleInstruction(c, offset);  
    }
    
}