
#include <stdio.h>
#include "debug.h"



int simpleInstruction(const char* name,int offset)
{
    printf("%s\n", name);
    return offset + 1;
} 

int disassembleInstruction(Chunk* c, int offset) 
{
    printf("%04d ", offset);

    uint8_t instruction = c->code[offset];
    switch (instruction)
    {
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
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