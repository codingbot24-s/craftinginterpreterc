#include <stdlib.h>

#include "memory.h"

void* reallocate(void *pointer, int old_size, int new_size) 
{
    if (new_size == 0)
    {
        free(pointer);
        return NULL;
    }
    
    void* result = realloc(pointer,new_size);
    if (result == NULL)
    {
        // we can add the log like print   
        exit(1);
    }
    
    return result;
}
