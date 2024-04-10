#ifndef __CLOX_VM_H__
#define __CLOX_VM_H__

#include "value.h"
#include "chunk.h"

#define MAX_STACK 256
// Test commit
typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[MAX_STACK];
    Value* stackTop;
    Obj* objects;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);

void push(Value val);
Value pop();

extern VM vm;

#endif
