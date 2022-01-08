#ifndef PARSER
#define PARSER
#include "AST.h"
#include "lexic.h"


AST_NODE * program(Lexic * lexics);
AST_NODE * parse(Lexic * lexics);
ast_list * argDecl(Lexic * lexics);
ast_list * funcBody(Lexic * lexics);
ast_list * callArguments(Lexic * lexics);
AST_NODE * ifStatement(Lexic * lexics);
AST_NODE * variableDeclaration(Lexic * lexics);
ast_list * argDecl(Lexic * lexics);

#endif