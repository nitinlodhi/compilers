#ifndef __CLOX_COMPILER_H__
#define __CLOX_COMPILER_H__

#include "vm.h"

bool compile(const char* source, Chunk* chunk);

#endif