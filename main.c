
#include "chunk.h"
#include "debug.h"


int main (int argc, char** argv) 
{
    Chunk c;
    init_chunk(&c);


    /*
        [ OPCODE, OPRAND ]
    */

    int constant = add_constant(&c,1.2);
    write_chunk(&c,OP_CONSTANT);
    write_chunk(&c,constant);


    write_chunk(&c,OP_RETURN);
    disassembleChunk(&c,"test chunk");
    free_chunk(&c); 
    
    return 0;
}
