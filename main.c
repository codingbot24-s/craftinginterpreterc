
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
    write_chunk(&c,OP_CONSTANT,123);
    write_chunk(&c,constant,123);


    write_chunk(&c,OP_RETURN,123);
    disassembleChunk(&c,"test chunk");
    free_chunk(&c); 
    
    return 0;
}
