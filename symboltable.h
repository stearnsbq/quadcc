#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE
#include "table.h"
#include "AST.h"
#include <stdlib.h>
#define HASH_SIZE 25


typedef struct SymbolTable{
    char * scope;
    HashTable * table;
    struct SymbolTable * parent;
    struct SymbolTable * children;
    struct SymbolTable * next;
} SymbolTable;



int addChild(SymbolTable * parent, SymbolTable * child);

int symLookup(SymbolTable * table, char * identifier);

SymbolTable * allocateNode(size_t hashSize, char * scope, SymbolTable * parent);

SymbolTable * buildSymbolTable(AST_NODE * ast);
#endif