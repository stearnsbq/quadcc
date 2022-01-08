#include "parser.h"
#include <stdio.h>
#include <stdlib.h>


ast_list * argDecl(){

    if(lexics->token == RIGHT_PARENTHESIS){
        return NULL;
    }

    ast_list * head = malloc(sizeof(ast_list));

    char *type;

    if(lexics->token != VARTYPE){
        printf("Data Type expected!\n");
        return NULL;
    }
    type = lexics->lexeme;
    lexics++;

    if(lexics->token != IDENTIFIER){
        printf("Identifier expected!\n");
        return NULL;
    }

    AST_NODE * decl = make_variableDecl(type, lexics->lexeme, NULL, NULL, NULL, NULL);
    head->elem = decl;
    lexics++;

    if(lexics->token == RIGHT_PARENTHESIS){
        head->next = NULL;
        return head;
    }
    lexics++;

    if(lexics->token != COMMA){
        printf(", expected\n");
        return NULL;
    }

    lexics++;

    if(lexics->token != RIGHT_PARENTHESIS){
        printf(") expected\n");
        return NULL;
    }

    head->next = argDecl(lexics);
    return head;
}

AST_NODE * funcDecl(){

    char *name;
    char *returnType;

    if(lexics->token != VARTYPE){
        printf("Data Type expected!\n");
        return NULL;
    }
    returnType = lexics->lexeme;
    lexics++;

    if(lexics->token != IDENTIFIER){
        printf("Identifier expected!\n");
        return NULL;
    }
    name = lexics->lexeme;
    lexics++;

    if(lexics->token != LEFT_PARENTHESIS){
        printf("left parenthesis expected!\n");
        return NULL;
    }

    lexics++;

    ast_list * params = argDecl(lexics);

    lexics++;

    ast_list * body = funcBody(lexics);

    lexics++;

    return make_funcDecl(name, params, returnType, body);
}

ast_list * funcBody(){

    if(lexics->token != LEFT_BRACKET){
        printf("{ expected!\n");
        return NULL;
    }
    lexics++;

    ast_list * head = malloc(sizeof(ast_list));

    ast_list * cursor = head;

    while(1){

        AST_NODE * current = NULL;

        switch(lexics->token){
            case VARTYPE:{
                current = variableDeclaration();
                break;
            }
            case IF_KEYWORD:{
                break;
            }
            case RETURN_KEYWORD:{
                break;
            }
        }

        if(current == NULL){
            break;
        }

        cursor->elem = current;
        cursor->next = malloc(sizeof(ast_list));
        cursor = cursor->next;
        lexics++;
    }

    return head;
}

AST_NODE * variableDeclaration(){

    char * name = NULL;
    char * type = NULL;
    AST_NODE * func = NULL;
    int intValue;
    char charValue;
    char * variable = NULL;


    if(lexics->token != VARTYPE){
        printf("type expected!\n");
        return NULL;
    }
    type = lexics->lexeme;
    lexics++;

    if(lexics->token != IDENTIFIER){
        printf("identifier expected!\n");
        return NULL;
    }
    name = lexics->lexeme;
    lexics++;

    if(lexics->token == EOL){
        return make_variableDecl(type, name, NULL, NULL, NULL, NULL);
    }

    if(lexics->token != EQUAL){
        printf("; expected!\n");
        return NULL;
    }
    lexics++;


    switch(lexics->token){
        case NUMBER:{
            intValue = atoi(lexics->lexeme);
            break;
        }
        case IDENTIFIER:{

            char * name = lexics->lexeme;

            lexics++;

            if(lexics->token == LEFT_PARENTHESIS){ // function call

                ast_list * args = argDecl();

                lexics++;

                func = make_callExp(name, args);

                if(lexics->token != RIGHT_PARENTHESIS){
                    printf(") expected\n");
                    return NULL;
                }
                
                break;
            }


            variable = lexics->lexeme;

            break;
        }
        case CHARACTER:{
            charValue = lexics->lexeme[0];
           break; 
        }
    }
    lexics++;


    if(lexics->token != EOL){
        printf("; expected!\n");
        return NULL; 
    }

    return make_variableDecl(type, name, func, intValue, charValue, variable);
}


AST_NODE * ifStatement(){

    if(lexics->token != IF_KEYWORD){
        return NULL;
    }
    lexics++;


    if(lexics->token != LEFT_PARENTHESIS){
        printf("( expected");
        return NULL;
    }
    lexics++;


    AST_NODE * test = binaryExp();
    lexics++;


    return make_IfStatement(test, NULL, NULL);
}

AST_NODE * binaryExp(){

    char * operator;

    AST_NODE * left;
    AST_NODE * right;

    if(lexics->token != IDENTIFIER || lexics->token != NUMBER){
        printf("identifier or number expected\n");
        return NULL;
    }




}


AST_NODE * program(){

    ast_list * head = malloc(sizeof(ast_list));

    ast_list * cursor = head;

    while(lexics->token != ENDOFFILE){

        AST_NODE * tmp = funcDecl(lexics);

        if(!tmp){
            break;
        }

        cursor->elem = tmp;

        cursor->next = malloc(sizeof(ast_list));

        cursor = cursor->next;

        lexics++;

    }


    return make_progamDecl("test", head);
}


AST_NODE * parse(Lexic * lexicsIn){
    lexics = lexicsIn;
    return program(lexics);
}