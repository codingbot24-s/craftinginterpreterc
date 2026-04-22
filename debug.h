#ifndef DEBUG_H

#define DEBUG_H
#include "chunk.h"

void disassembleChunk(Chunk* c,const char* name);
int disassembleInstruction(Chunk* c, int offset);

#endif // DEBUG_H 
