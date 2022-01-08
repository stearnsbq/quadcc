#ifndef PARSER
#define PARSER
#include "AST.h"
#include "lexic.h"


AST_NODE program(Lexic * lexics);
AST_NODE parse(Lexic * lexics);

#endif