#ifndef ASM_H
#define ASM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

// Gera o código assembly x86_64 a partir do código intermediário ILOC
char *generate_asm(INSTRUCTION *inst);

// Traduz uma instrução ILOC para x86_64
char *ILOCtoX86(INSTRUCTION *inst);

#endif