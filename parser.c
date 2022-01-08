#include "parser.h"
#include <stdio.h>


AST_NODE * argDecl(Lexic * lexics){

    if(lexics->token != VARTYPE){
        printf("Data Type expected!\n");
        return NULL;
    }
    lexics++;

    if(lexics->token != IDENTIFIER){
        printf("Identifier expected!\n");
        return NULL;
    }
    lexics++;

    if(lexics->token == RIGHT_PARENTHESIS){
        return 0;
    }


    if(lexics->token != COMMA){
        printf(", expected\n");
        return NULL;
    }

    lexics++;

    return argDecl(lexics);
}

AST_NODE * funcDecl(Lexic * lexics){

    if(lexics->token != VARTYPE){
        printf("Data Type expected!\n");
        return NULL;
    }
    lexics++;

    if(lexics->token != IDENTIFIER){
        printf("Identifier expected!\n");
        return NULL;
    }
    lexics++;

    if(lexics->token != LEFT_PARENTHESIS){
        printf("Identifier expected!\n");
        return NULL;
    }
    lexics++;


    if(lexics->token == RIGHT_PARENTHESIS){
        return 0;
    }

    return argDecl(lexics);

}


AST_NODE * funcBody(Lexic * lexics){

}


AST_NODE * program(Lexic * lexics){

    if(!funcDecl(lexics)){
        printf("Function Declaration Missing!\n");
        return NULL;
    }
    lexics++;

    if(!funcBody(lexics)){
        printf("Body expected\n");
        return NULL;
    }


    if(lexics->token > 0){
        return NULL;
    }

    
    return node;

}


AST_NODE * parse(Lexic * lexics){

    return program(lexics);
}