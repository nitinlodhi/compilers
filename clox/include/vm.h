#ifndef __CLOX_VM_H__
#define __CLOX_VM_H__

#include "value.h"
#include "chunk.h"

#define MAX_STACK 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[MAX_STACK];
    Value* stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);

void push(Value val);
Value pop();

#endif