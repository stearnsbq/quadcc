#include "AST.h"
#include <stdlib.h>


AST_NODE *make_integerExp(int value){
    AST_NODE * newNode = malloc(sizeof(AST_NODE));

    newNode->tag = integer_exp;
    newNode->op.integerExp = value;

    return newNode;
}

AST_NODE *make_variableDecl(char *type, char *identifier, AST_NODE * func, int intValue, char charValue, char * variable){
    AST_NODE * newNode = malloc(sizeof(AST_NODE));

    newNode->tag = variable_decl;

    newNode->op.variableDecl.identifier = identifier;
    //newNode->op.variableDecl.init.charValue = charValue;
    newNode->op.variableDecl.init.intValue = intValue;
    //newNode->op.variableDecl.init.func = func;
    //newNode->op.variableDecl.init.variable = variable;

    if(func != NULL || intValue != MAX_INT || charValue != NULL || variable != NULL){
        newNode->op.variableDecl.initalized = 1;
    }
    
    newNode->op.variableDecl.type = type;

    return newNode;
}

AST_NODE *make_IfStatement(AST_NODE *test, ast_list *consequent, ast_list *alternate){
    AST_NODE * newNode = malloc(sizeof(AST_NODE));

    newNode->tag = if_stmt;

    newNode->op.ifStatement.test = test;
    newNode->op.ifStatement.consequent = consequent;
    newNode->op.ifStatement.alternate = alternate;

    return newNode;
}

AST_NODE *make_progamDecl(char *name, AST_NODE * main, struct Exp_list *body){
    AST_NODE * newNode = malloc(sizeof(AST_NODE));

    newNode->tag = program_decl;

    newNode->op.programDecl.body = body;
    newNode->op.programDecl.main = main;
    newNode->op.programDecl.name = name;


    return newNode;
}

AST_NODE *make_binaryExp(char *operator, AST_NODE * left, AST_NODE *right){
    AST_NODE * newNode = malloc(sizeof(AST_NODE));

    newNode->tag = binary_exp;

    newNode->op.binaryExp.operator = operator;
    newNode->op.binaryExp.left = left;
    newNode->op.binaryExp.right = right;


    return newNode;
}

AST_NODE *make_funcDecl(char *name, struct Exp_list *params, char *returnType, struct Exp_list *body){
    AST_NODE * newNode = malloc(sizeof(AST_NODE));

    newNode->tag = function_decl;

    newNode->op.funcDecl.name = name;
    newNode->op.funcDecl.params = params;
    newNode->op.funcDecl.returnType = returnType;
    newNode->op.funcDecl.body = body;

    return newNode;
}

AST_NODE *make_callExp(char* callee, struct Exp_list *arguments){
    AST_NODE * newNode = malloc(sizeof(AST_NODE));

    newNode->tag = call_exp;

    newNode->op.callExp.arguments = arguments;
    newNode->op.callExp.callee = callee;

    return newNode;
}