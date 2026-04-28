
#include "chunk.h"
#include "memory.h"
#include <stdlib.h>

void init_chunk(Chunk *c)
{
    c->capacity = 0;
    c->count = 0;
    c->code = NULL;
    c->line = NULL;
    init_valuearr(&c->constants);
}

void write_chunk(Chunk *c, uint8_t byte,int line)
{
    if (c->capacity < c->count + 1)
    {
        int old_capacity = c->capacity;
        c->capacity = GROW_CAPACITY(old_capacity);
        c->code = GROW_ARRAY(uint8_t,c->code,old_capacity,c->capacity);
        c->line = GROW_ARRAY(int,c->line,old_capacity,c->capacity);
    }
    
    c->code[c->count] = byte;
    c->line[c->count] = line;
    c->count++;
}

void free_chunk(Chunk* c) 
{
    FREE_ARRAY(uint8_t,c->code,c->capacity);
    FREE_ARRAY(int,c->line,c->capacity);
    free_valuearr(&c->constants);
    init_chunk(c);
}


// add_constant add the value in the value pool and return the index
int add_constant(Chunk *c, Value constant)
{
    write_valuearr(&c->constants,constant);
    return c->constants.count - 1;
}
