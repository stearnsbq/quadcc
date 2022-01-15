#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

AST_NODE *funcCall()
{

    if (lexics->token != IDENTIFIER)
    {
        return NULL;
    }
    char *callee = lexics->lexeme;
    lexics++;

    if (lexics->token != LEFT_PARENTHESIS)
    {
        return NULL;
    }
    lexics++;

    AST_NODE *func = malloc(sizeof(AST_NODE));

    ast_list *args = callArgs();

    if (lexics->token != RIGHT_PARENTHESIS)
    {
        printf(") expected\n");
        return NULL;
    }

    return make_callExp(callee, args);
}

ast_list *callArgs()
{
    if (lexics->token == RIGHT_PARENTHESIS)
    {
        return NULL;
    }

    Lexic * state = lexics;

    ast_list *head = malloc(sizeof(ast_list));

    ast_list *cursor = head;

    while (lexics->token != RIGHT_PARENTHESIS)
    {

        switch (lexics->token)
        {
        case IDENTIFIER:
        {

            if ((lexics + 1)->token == LEFT_PARENTHESIS)
            {
                cursor->elem = funcCall();
                break;
            }

            cursor->elem = make_variableDecl(NULL, lexics->lexeme, NULL, NULL, NULL, NULL);

            break;
        }
        case NUMBER:
        {
            cursor->elem = make_integerExp(atoi(lexics->lexeme));
            break;
        }
        }

        lexics++;

        if(lexics->token == BINOP){ // if we find a binary op rollback

            free(cursor->elem);
            
            lexics = state;

            cursor->elem = binaryExp();

            state = lexics;
        }


        if(lexics->token != RIGHT_PARENTHESIS){
            cursor->next = malloc(sizeof(ast_list));
            cursor = cursor->next;
        }


    }
    return head;
}

ast_list *argDecl()
{

    if (lexics->token == RIGHT_PARENTHESIS)
    {
        return NULL;
    }

    ast_list *head = malloc(sizeof(ast_list));

    char *type;

    if (lexics->token != VARTYPE)
    {
        printf("Data Type expected!\n");
        return NULL;
    }
    type = lexics->lexeme;
    lexics++;

    if (lexics->token != IDENTIFIER)
    {
        printf("Identifier expected!\n");
        return NULL;
    }

    AST_NODE *decl = make_variableDecl(type, lexics->lexeme, NULL, NULL, NULL, NULL);
    head->elem = decl;
    lexics++;

    if (lexics->token == RIGHT_PARENTHESIS)
    {
        head->next = NULL;
        return head;
    }
    lexics++;

    if (lexics->token != COMMA)
    {
        printf(", expected\n");
        return NULL;
    }

    lexics++;

    if (lexics->token != RIGHT_PARENTHESIS)
    {
        printf(") expected\n");
        return NULL;
    }

    head->next = argDecl(lexics);
    return head;
}

AST_NODE *funcDecl()
{

    char *name;
    char *returnType;

    if (lexics->token != VARTYPE)
    {
        printf("Data Type expected!\n");
        return NULL;
    }
    returnType = lexics->lexeme;
    lexics++;

    if (lexics->token != IDENTIFIER)
    {
        printf("Identifier expected!\n");
        return NULL;
    }
    name = lexics->lexeme;
    lexics++;

    if (lexics->token != LEFT_PARENTHESIS)
    {
        printf("left parenthesis expected!\n");
        return NULL;
    }

    lexics++;

    ast_list *params = argDecl(lexics);

    lexics++;

    ast_list *body = funcBody(lexics);

    lexics++;

    return make_funcDecl(name, params, returnType, body);
}

ast_list *funcBody()
{

    if (lexics->token != LEFT_BRACKET)
    {
        printf("{ expected!\n");
        return NULL;
    }
    lexics++;

    ast_list *head = malloc(sizeof(ast_list));

    ast_list *cursor = head;

    while (lexics->token != RIGHT_BRACKET)
    {

        AST_NODE *current = NULL;

        switch (lexics->token)
        {
        case VARTYPE:
        {
            current = variableDeclaration();
            break;
        }
        case IF_KEYWORD:
        {
            current = ifStatement();
            break;
        }
        case RETURN_KEYWORD:
        {
            current = returnStatement();
            break;
        }
        }

        if (current == NULL)
        {
            break;
        }

        cursor->elem = current;

        lexics++;
        
        if (lexics->token != RIGHT_BRACKET)
        {
            cursor->next = malloc(sizeof(ast_list));
            cursor = cursor->next;
        }


    }

    return head;
}

AST_NODE *variableDeclaration()
{

    char *name = NULL;
    char *type = NULL;
    AST_NODE *func = NULL;
    int intValue;
    char charValue;
    char *variable = NULL;

    if (lexics->token != VARTYPE)
    {
        printf("type expected!\n");
        return NULL;
    }
    type = lexics->lexeme;
    lexics++;

    if (lexics->token != IDENTIFIER)
    {
        printf("identifier expected!\n");
        return NULL;
    }
    name = lexics->lexeme;
    lexics++;

    if (lexics->token == EOL)
    {
        return make_variableDecl(type, name, NULL, NULL, NULL, NULL);
    }

    if (lexics->token != EQUAL)
    {
        printf("; expected!\n");
        return NULL;
    }
    lexics++;

    switch (lexics->token)
    {
    case NUMBER:
    {
        intValue = atoi(lexics->lexeme);
        break;
    }
    case IDENTIFIER:
    {

        if ((lexics + 1)->token == LEFT_PARENTHESIS)
        { // function call

            func = funcCall();

            break;
        }

        variable = lexics->lexeme;

        break;
    }
    case CHARACTER:
    {
        charValue = lexics->lexeme[0];
        break;
    }
    }
    lexics++;

    if (lexics->token != EOL)
    {
        printf("; expected!\n");
        return NULL;
    }

    return make_variableDecl(type, name, func, intValue, charValue, variable);
}

AST_NODE *ifStatement()
{

    if (lexics->token != IF_KEYWORD)
    {
        return NULL;
    }
    lexics++;

    if (lexics->token != LEFT_PARENTHESIS)
    {
        printf("( expected");
        return NULL;
    }
    lexics++;

    AST_NODE *test = binaryExp();

    if (lexics->token != RIGHT_PARENTHESIS)
    {
        printf(") expected");
        return NULL;
    }
    lexics++;

    if (lexics->token != LEFT_BRACKET)
    {
        printf("{ expected");
        return NULL;
    }

    ast_list *consequent = funcBody();

    //TODO: if/else else support

    return make_IfStatement(test, consequent, NULL);
}

AST_NODE *returnStatement()
{

    if (lexics->token != RETURN_KEYWORD)
    {
        printf("return expected\n");
        return NULL;
    }
    lexics++;

    Lexic* state = lexics;

    AST_NODE *returnNode = malloc(sizeof(AST_NODE));

    returnNode->tag = return_stmt;

    if (lexics->token == EOL)
    {
        returnNode->tag = return_stmt;
        return returnNode;
    }

    if ((lexics + 1)->token == BINOP)
    {
        returnNode->op.returnStatement = binaryExp();
    }
    else
    {

        if (lexics->token == IDENTIFIER)
        {

            if ((lexics + 1)->token == LEFT_PARENTHESIS)
            {

                returnNode->op.returnStatement = funcCall();
            }
            else
            {
                returnNode->op.returnStatement = make_variableDecl(NULL, lexics->lexeme, NULL, NULL, NULL, NULL);
            }
        }
        else if (lexics->token == NUMBER)
        {
            returnNode->op.returnStatement = make_integerExp(atoi(lexics->lexeme));
        }
    }

    lexics++;

    if(lexics->token == BINOP){

            free(returnNode->op.returnStatement);
            
            lexics = state;

            returnNode->op.returnStatement = binaryExp();

            state = lexics;

    }

    if (lexics->token != EOL)
    {
        printf("; expected\n");
        return NULL;
    }

    return returnNode;
}

AST_NODE *binaryExpBranch()
{

    switch (lexics->token)
    {
    case IDENTIFIER:
    {

        if ((lexics + 1)->token == LEFT_PARENTHESIS)
        {
            return funcCall();
        }

        return make_variableDecl(NULL, lexics->lexeme, NULL, NULL, NULL, NULL);
    }
    case NUMBER:
    {
        return make_integerExp(atoi(lexics->lexeme));
    }
    }
}

AST_NODE *binaryExp()
{

    char *operator;

    AST_NODE *left;
    AST_NODE *right;

    left = binaryExpBranch();
    lexics++;

    if (lexics->token != BINOP)
    {
        printf("binary operator expected\n");
        return NULL;
    }
    operator= lexics->lexeme;
    lexics++;

    right = binaryExpBranch();
    lexics++;

    AST_NODE *node = make_binaryExp(operator, left, right);

    if (lexics->token == BINOP)
    {
        operator= lexics->lexeme;
        lexics++;
        left = node;
        right = binaryExp();

        return make_binaryExp(operator, left, right);
    }

    return node;
}

AST_NODE *program()
{

    ast_list *head = malloc(sizeof(ast_list));

    ast_list *cursor = head;

    while (lexics->token != ENDOFFILE)
    {

        AST_NODE *tmp = funcDecl(lexics);

        if (!tmp)
        {
            break;
        }

        cursor->elem = tmp;

        if(lexics->token != ENDOFFILE){
            cursor->next = malloc(sizeof(ast_list));
            cursor = cursor->next;
        }

        //lexics++;
    }

    return make_progamDecl("test", head);
}

AST_NODE *parse(Lexic *lexicsIn)
{
    lexics = lexicsIn;
    lexicsHead = lexicsIn;
    return program(lexics);
}