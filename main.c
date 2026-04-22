
#include "chunk.h"
#include "debug.h"


int main (int argc, char** argv) 
{
    Chunk c;
    init_chunk(&c);
    write_chunk(&c,OP_RETURN);
    disassembleChunk(&c,"test chunk");
    free_chunk(&c); 
    
    return 0;
}
