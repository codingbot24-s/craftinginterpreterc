#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>

// instructions 
typedef enum {
    OP_RETURN
}OPCODE;


// chunk array 
typedef struct Chunk 
{

    int count;
    int capacity;
    uint8_t* code;

}Chunk;


void init_chunk(Chunk* c);
void write_chunk(Chunk* c,uint8_t byte);
void free_chunk(Chunk* c);

#endif // CHUNK_H