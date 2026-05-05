#include "value.h"
#include "stdlib.h"
#include "memory.h"

#include <stdio.h>

void init_valuearr(ValueArray* varr)
{
    varr->capacity = 0;
    varr->count    = 0;
    varr->Values   = NULL;
}



void write_valuearr(ValueArray *varr, Value value)
{
    if (varr->capacity < varr->count + 1)
    {
        int old_capacity = varr->capacity;
        varr->capacity = GROW_CAPACITY(old_capacity);
        varr->Values = GROW_ARRAY(Value,varr->Values,old_capacity,varr->capacity);
    }
    
    varr->Values[varr->count] = value;
    varr->count++;    
}

void free_valuearr(ValueArray *varr)
{
    FREE_ARRAY(Value,varr->Values,varr->capacity);
    init_valuearr(varr);
}

void print_value(Value value) {
    printf("%g", AS_NUMBER(value));
}
