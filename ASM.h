#ifndef ASM_H
#define ASM_H

#include "symboltable.h"
#include "AST.h"


void generateASM(SymbolTable * symbolTable, AST_NODE * ast);

#endif