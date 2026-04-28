#ifndef COMPILER_H

#define COMPILER_H

#include <stdbool.h>
#include "vm.h"


bool compile(const char* source,Chunk* c);

#endif //COMPILER_H