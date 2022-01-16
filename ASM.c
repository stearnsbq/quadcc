#include "ASM.h"
#include <stdio.h>
#include <string.h>

void handleASMFuncDecl(FILE * file, SymbolTable * table, AST_NODE * func){

    char * funcName = func->op.funcDecl.name;

    fprintf(file, "%s\t", funcName); // write function name

    struct Exp_list * body = func->op.funcDecl.body;

    struct Exp_list * cursor = body;

    while(cursor != NULL){

        AST_NODE * elem = cursor->elem;

        switch(elem->tag){
            case variable_decl:{

                

                break;
            }
        }

        cursor = cursor->next;
    }



    fprintf(file, "RET\n");
}


void handleMainFunc(FILE * file, SymbolTable * symbolTable, AST_NODE * main){


    char * mainHeader = "AND R5, R5 #0\n" // clear registers we need
                        "AND R6, R6 #0\n"
                        "AND R7, R7 #0\n"

                        "LD R6, STACK\n" // load base stack address
                        "LD R5, STACK\n"

                        "ADD R6, R6, #-3\n" // allocate space for return value, return address and frame pointer

                        "STR R7, R6, #1\n" // save return address 

                        "STR R5, R6, #0\n" // save caller frame pointer

                        "ADD R5, R6, #0\n"; // local frame pointer


    fprintf(file, mainHeader);


    struct Exp_list * mainBody = main->op.funcDecl.body;

    struct Exp_list * cursor = mainBody;

    while(cursor != NULL){

        AST_NODE * elem = cursor->elem;

        switch(elem->tag){
            case variable_decl:{

                if(elem->op.variableDecl.initalized){

                    fprintf(file, "ADD R6, R6, #-1\n"); // allocate space on stack for variable

                    fprintf(file, "%s .FILL #%d\n", elem->op.variableDecl.identifier, elem->op.variableDecl.init.intValue); // no move instruction so we have to fill

                    fprintf(file, "LD R7, %s\n", elem->op.variableDecl.identifier);

                    fprintf(file, "STR R7, R6, #0\n");

                }


                
                break;  
            }
        }

        cursor = cursor->next;
    }





}

void generateASM(SymbolTable * symbolTable, AST_NODE * ast){
    FILE * file = fopen("a.out", "w+");

    if(file == NULL){
        printf("failed to open file\n");
        exit(1);
    }

    // write .ORIG pseudo op 
    fprintf(file, ".ORIG x3000\n");

    AST_NODE * main = ast->op.programDecl.main;

    if(main == NULL){
        printf("Missing main entrypoint function!\n");
        fclose(file);
        return exit(1);
    }

    // setup main func

    handleMainFunc(file, symbolTable, main);


    // handle other funcs
    ast_list *programBody = ast->op.programDecl.body;

    ast_list *cursor = programBody;

    while (cursor != NULL)
    {

        AST_NODE *node = cursor->elem;

        switch (node->tag)
        {
        case function_decl:
        {
            handleASMFuncDecl(file, symbolTable, node);
            break;
        }
        }

        cursor = cursor->next;
    }

  


    fprintf(file, "HALT\n");

    // store stack base address and stack limit
    fprintf(file, "STACK .FILL x7FFF\n");
    fprintf(file, "MAX_STACK .FILL x7000\n");


    fprintf(file, ".END");

    fclose(file);

}