#include "parser.h"
#include <stdio.h>



AST_NODE program(Lexic * lexic){


    if(lexic->token != VARTYPE){
        printf("Data Type expected!\n");
        return NULL;
    }

}


AST_NODE parse(Lexic * lexics, int length){

    return program(lexics);
}