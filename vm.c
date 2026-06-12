#include "vm.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include "chunk.h"
#include "common.h"
#include "debug.h"
#include <stdbool.h>
#include <string.h>
#include "compiler.h"
#include "object.h"
#include "value.h"
#include "memory.h"


VM vm;

void static reset_stack()
{
    vm.stack_top = vm.stack;
}

void init_vm()
{
    reset_stack();
}

void free_vm()
{
}


// this function is in value.c in book
bool values_equal(Value a, Value b)
{
    if (a.type != b.type)
    {
        return false;
    }

    switch (a.type)
    {
    case VAL_BOOL:
        return AS_BOOL(a) == AS_BOOL(b);
        break;
    case VAL_NIL:
        return true;
    case VAL_NUMBER:
        return AS_NUMBER(a) == AS_NUMBER(b);
    case VAL_OBJ : {
        obj_string* aString = AS_STRING(a);
        obj_string* bString = AS_STRING(b);

        return aString->len == bString->len && memcpy(aString->chars, bString->chars, aString->len) == 0;
    }
    default:
        return false;
    }
}

static void runtime_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->code - 1;
    int line = vm.chunk->line[instruction];
    fprintf(stderr, "[line %d] in script\n", line);
    reset_stack();
}

static bool is_falsely(Value value)
{
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void concatenate() 
{
    obj_string* stringb = AS_STRING(pop());    
    obj_string* stringa = AS_STRING(pop());    


    int len = stringa->len = stringb->len;
    
    char* chars = ALLOCATE(char,len + 1);
    memcpy(chars,stringa->chars,stringa->len);
     memcpy(chars + stringa->len,stringb->chars,stringb->len);
   
    chars[len] = '\0';
    obj_string* result = take_string(chars,len);

    push(OBJ_VAL(result));
}



static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.Values[READ_BYTE()])

    /*

        BINARY OPS MACRO


    */

#define BINARY_OP(valueType, op)                        \
    do                                                  \
    {                                                   \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) \
        {                                               \
            runtime_error("Operands must be numbers."); \
            return INTERPRET_RUNTIME_ERROR;             \
        }                                               \
        double b = AS_NUMBER(pop());                    \
        double a = AS_NUMBER(pop());                    \
        push(valueType(a op b));                        \
    } while (false)

    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION

        printf("          ");
        for (Value *slot = vm.stack; slot < vm.stack_top; slot++)
        {
            printf("[ ");
            print_value(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));

#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE())
        {

        case OP_NIL:
            push(NIL_VAL);
            break;
        case OP_TRUE:
            push(BOOL_VAL(true));
            break;
        case OP_FALSE:
            push(BOOL_VAL(false));
            break;
        case OP_CONSTANT:
            Value constant = READ_CONSTANT();
            push(constant);
            break;
        case OP_NEGATE:
            if (!IS_NUMBER(peek(0)))
            {
                runtime_error("Operand must be a number");
                return INTERPRET_RUNTIME_ERROR;
            }
            push(NUMBER_VAL(-AS_NUMBER(pop())));
            break;
        case OP_ADD:
            /// TODO: add number runtime error fix it     
            if (IS_STRING(peek(0)) && IS_STRING(peek(1)))
            { 
                concatenate();        
            }
            // why binary op macro is not prsent here in book? 
            BINARY_OP(NUMBER_VAL, +);
            break;
        case OP_SUB:
            BINARY_OP(NUMBER_VAL, -);
            break;
        case OP_MUL:
            BINARY_OP(NUMBER_VAL, *);
            break;
        case OP_DIV:
            BINARY_OP(NUMBER_VAL, /);
            break;
        case OP_NOT:
            push(BOOL_VAL(is_falsely(pop())));
            break;
        case OP_EQUAL:
            Value b = pop();
            Value a = pop();
            push(BOOL_VAL(values_equal(a, b)));
            break;

        case OP_GREATER:
            BINARY_OP(BOOL_VAL,>);
            break;
        case OP_LESS:
            BINARY_OP(BOOL_VAL,<);
            break;    
        case OP_RETURN:
            print_value(pop());
            printf("\n");
            return INTERPRET_OK;
            // NOTE: implementation of peek has changed
            // case OP_DUP:
            //  Value value = peek();
            // push(value);
            // break;
        }
    }
}

InterpretResult interpret(const char *source)
{
    Chunk c;
    init_chunk(&c);

    if (!compile(source, &c))
    {
        free_chunk(&c);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &c;
    vm.ip = vm.chunk->code;

    InterpretResult res = run();
    free_chunk(&c);
    return res;
}

void push(Value value)
{
    *vm.stack_top = value;
    vm.stack_top++;
}

Value pop()
{
    vm.stack_top--;
    return *vm.stack_top;
}

Value peek(int distance)
{
    return vm.stack_top[-1 - distance];
}
