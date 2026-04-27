#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>
#include "value.h"

// instructions 
typedef enum {
    OP_RETURN,
    OP_CONSTANT,
    OP_NEGATE,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    // TESTING
    OP_DUP,
}OPCODE;


// chunk array 
typedef struct Chunk 
{

    int count;
    int capacity;
    uint8_t* code;
    /* line number array inefficient with memory ? */
    int* line;
    /* chunk will have its own constant array */  
    ValueArray constants;

}Chunk;


void init_chunk(Chunk* c);
void write_chunk(Chunk* c,uint8_t byte,int line);
void free_chunk(Chunk* c);

int add_constant(Chunk* c,Value constant);

#endif // CHUNK_H