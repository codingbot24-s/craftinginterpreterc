
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main (int argc, char** argv) 
{
    init_vm();
    Chunk c;
    init_chunk(&c);


    /*
        [ OPCODE, OPRAND ]
    */
    // write the constant 
    int constant = add_constant(&c,1.2);
    write_chunk(&c,OP_CONSTANT,123);
    write_chunk(&c,constant,123);


    write_chunk(&c,OP_RETURN,123);
    disassembleChunk(&c,"test chunk");
    interpret(&c);
    free_vm();
    free_chunk(&c); 
    
    return 0;
}
