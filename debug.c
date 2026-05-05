
#include <stdio.h>
#include "debug.h"
#include "chunk.h"
#include "value.h"

int simpleInstruction(const char *name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

int constant_instruction(const char *name, Chunk *c, int offset)
{
    uint8_t c_index = c->code[offset + 1];
    // no new line buffered
    printf("%-16s %4d '", name, c_index);

    print_value(c->constants.Values[c_index]);
    printf("'\n");
    return offset + 2;
}

int disassembleInstruction(Chunk *c, int offset)
{
    printf("%04d ", offset);
    if (offset > 0 && c->line[offset] == c->line[offset - 1])
    {
        printf("   | ");
    }
    else
    {
        printf("%4d ", c->line[offset]);
    }

    uint8_t instruction = c->code[offset];
    switch (instruction)
    {
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    case OP_NEGATE:
        return simpleInstruction("OP_NEGATE", offset);
    case OP_ADD:
        return simpleInstruction("OP_ADD", offset);
    case OP_SUB:
        return simpleInstruction("OP_SUB", offset);
    case OP_MUL:
        return simpleInstruction("OP_MUL", offset);
    case OP_DIV:
        return simpleInstruction("OP_DIV", offset);
    case OP_DUP:
        return simpleInstruction("OP_DUP", offset);
    case OP_CONSTANT:
        return constant_instruction("OP_CONSTANT", c, offset);
    case OP_FALSE:
        return simpleInstruction("OP_FALSE", offset);
     case OP_TRUE:
         return simpleInstruction("OP_TRUE", offset);
    case OP_NIL:
        return simpleInstruction("OP_NIL", offset);

    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}

void disassembleChunk(Chunk *c, const char *name)
{
    printf("== %s ==\n", name);

    for (int offset = 0; offset < c->count;)
    {
        offset = disassembleInstruction(c, offset);
    }
}
