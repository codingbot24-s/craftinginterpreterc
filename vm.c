#include "vm.h"
#include <stdio.h>
#include "common.h"
#include "debug.h"

VM vm; 

static reset_stack()
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

static InterpretResult run () 
{
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.Values[READ_BYTE()])
    for(;;)
    {
        #ifdef DEBUG_TRACE_EXECUTION 
            disassembleInstruction(vm.chunk,(int)(vm.ip - vm.chunk->code));
        #endif
        uint8_t instruction;
        switch(instruction = READ_BYTE())
        {
            case OP_CONSTANT:
                Value constant = READ_CONSTANT();
                print_value(constant);
                printf("\n");
                break;
            case OP_RETURN:
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
}

Value pop()
{
    
}
