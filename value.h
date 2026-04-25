#ifndef VALUE_H

#define VALUE_H

typedef double Value;


/*
    For storing constant 
*/

typedef struct ValueArray
{

    int count;
    int capacity;
    Value* Values;

}ValueArray;


void init_valuearr(ValueArray* varr);
void write_valuearr(ValueArray* varr,Value value);
void free_valuearr(ValueArray* varr);
void print_value(Value v);

#endif //VALUE_H  