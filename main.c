
#include "chunk.h"

int main (int argc, char** argv) 
{
    Chunk c;
    init_chunk(&c);
    write_chunk(&c,OP_RETURN);
    free_chunk(&c); 
    
    return 0;
}
