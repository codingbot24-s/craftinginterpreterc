#ifndef VALUE_H

#define VALUE_H

#include <stdbool.h>


typedef enum
{
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJ,
} value_type;


typedef struct Obj Obj;
typedef struct obj_string o_string;


typedef struct
{
    value_type type;
    union
    {
        bool boolean;
        double number;
        Obj* obj;
    } as;
}Value;

#define BOOL_VAL(value)   ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL           ((Value){VAL_NIL, {.number= 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})
#define OBJ_VAL(object) ((Value){VAL_OBJ, {.obj = (Obj*)object}})


#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_OBJ(value) ((value).as.obj)


#define IS_BOOL(value)    ((value).type == VAL_BOOL)
#define IS_NIL(value)     ((value).type == VAL_NIL)
#define IS_NUMBER(value)  ((value).type == VAL_NUMBER)
#define IS_OBJ(value)  ((value).type == VAL_OBJ)
/*
    For storing constant
*/
typedef struct ValueArray
{

    int count;
    int capacity;
    Value *Values;

} ValueArray;

void init_valuearr(ValueArray *varr);
void write_valuearr(ValueArray *varr, Value value);
void free_valuearr(ValueArray *varr);
void print_value(Value v);

#endif // VALUE_H
