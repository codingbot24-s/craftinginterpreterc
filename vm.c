#include "vm.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include "chunk.h"
#include "common.h"
#include "debug.h"
#include <stdbool.h>
#include "compiler.h"
#include "value.h"

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
            runtime_error("Operands must be numbers.");  \
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
        switch (instruction = READ_BYTE()) {

            case OP_NIL:
                push(NIL_VAL);
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
        // NOTE: implementation of peek has changed
        // case OP_DUP:
        //  Value value = peek();
        // push(value);
        // break;
        case OP_RETURN:
            print_value(pop());
            printf("\n");
            return interprET_OK;
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
