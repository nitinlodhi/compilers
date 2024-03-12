#ifndef __CLOX_CHUNK_H__
#define __CLOX_CHUNK_H__

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    int* lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk*, uint8_t byte, int line);
void freeChunk(Chunk* chunk);
int addConstant(Chunk* chunk, Value value);

#endif