#ifndef PARSER
#define PARSER
#include "AST.h"
#include "lexic.h"


Lexic * lexics;

AST_NODE * program();
AST_NODE * parse(Lexic * lexicsIn);
ast_list * argDecl();
ast_list * funcBody();
ast_list * callArguments();
AST_NODE * ifStatement();
AST_NODE * variableDeclaration();
ast_list * argDecl();
AST_NODE * binaryExp();

#endif