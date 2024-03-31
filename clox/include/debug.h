#ifndef __CLOX_DEBUG_H__
#define __CLOX_DEBUG_H__

#include "chunk.h"

int disassembleInstruction(Chunk* chunk, int offset);
void disassembleChunk(Chunk* chunk, const char* name);
void displayChunk(Chunk* chunk, const char* name);
#endif