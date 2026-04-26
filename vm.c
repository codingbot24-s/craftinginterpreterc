#include "vm.h"
#include <stdio.h>
#include "common.h"
#include "debug.h"

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

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.Values[READ_BYTE()])
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
        case OP_CONSTANT:
            Value constant = READ_CONSTANT();
            push(constant);
            break;
        case OP_NEGATE: 
            push(-pop());
            break;
        case OP_RETURN:
            print_value(pop());
            printf("\n");
            return INTERPRET_OK;
        }
    }
}

InterpretResult interpret(Chunk *c)
{
    vm.chunk = c;
    vm.ip = vm.chunk->code;
    run();
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
