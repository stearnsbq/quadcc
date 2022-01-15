#include "symboltable.h"
#include <stdlib.h>
#include <stdio.h>

int symLookup(SymbolTable *table, char *identifier)
{

    if (has(table->table, identifier))
    {
        return 1;
    }
    else if (table->parent == NULL)
    {
        return 0;
    }

    return symLookup(table->parent, identifier);
}

void handleCallExp(SymbolTable *table, AST_NODE *exp)
{

    if (!symLookup(table, exp->op.callExp.callee))
    {
        printf("Undefined identifier %s\n", exp->op.variableDecl.identifier);
        exit(1);
    }

    // check arguments

    struct Exp_list *args = exp->op.callExp.arguments;

    struct Exp_list *cursor = args;

    while (cursor != NULL)
    {
        AST_NODE *elem = cursor->elem;

        switch (elem->tag)
        {
        case binary_exp:
        {
            handleBinaryExp(table, elem);
            break;
        }
        case variable_decl:
        {
            if (!symLookup(table, elem->op.variableDecl.identifier))
            {
                printf("Undefined identifier %s\n", elem->op.variableDecl.identifier);
                exit(1);
            }
            break;
        }
        case call_exp:
        {
            handleCallExp(table, elem);
            break;
        }
        }

        cursor = cursor->next;
    }
}

void handleBinExpSide(SymbolTable *table, AST_NODE *exp)
{

    switch (exp->tag)
    {
    case binary_exp:
    {
        handleBinaryExp(table, exp);
        break;
    }
    case variable_decl:
    {
        if (!symLookup(table, exp->op.variableDecl.identifier))
        {
            printf("Undefined identifier %s\n", exp->op.variableDecl.identifier);
            exit(1);
        }
        break;
    }
    case call_exp:
    {

        handleCallExp(table, exp);

        // check arguments

        break;
    }
    }
}

void handleBinaryExp(SymbolTable *table, AST_NODE *exp)
{

    AST_NODE *left = exp->op.binaryExp.left;

    handleBinExpSide(table, left);

    AST_NODE *right = exp->op.binaryExp.right;

    handleBinExpSide(table, right);
}

void handleReturn(SymbolTable *table, AST_NODE *returnBody)
{

    switch (returnBody->tag)
    {
    case variable_decl:
    {

        if (!symLookup(table, returnBody->op.variableDecl.identifier))
        {
            printf("Undefined identifier %s, in scope %s\n", returnBody->op.variableDecl.identifier, table->scope);
            exit(1);
        }

        break;
    }
    case binary_exp:
    {
        handleBinaryExp(table, returnBody);
        break;
    }
    case call_exp:
    {
        break;
    }
    }
}

void handleBlock(SymbolTable *table, AST_NODE *body)
{

    switch (body->tag)
    {
    case if_stmt:
    {

        AST_NODE *test = body->op.ifStatement.test;

        handleBinaryExp(table, test);

        ast_list *consequent = body->op.ifStatement.consequent;

        ast_list *cursor = consequent;

        SymbolTable *ifStmt = allocateNode(HASH_SIZE, "if", table);

        while (cursor != NULL)
        {
            handleBlock(ifStmt, cursor->elem);
            cursor = cursor->next;
        }

        break;
    }
    case return_stmt:
    {
        handleReturn(table, body->op.returnStatement);
        break;
    }
    case variable_decl:
    {
        set(table->table, body->op.variableDecl.identifier, body->op.variableDecl.type);
        break;
    }
    }
}

void handleFuncDecl(SymbolTable *table, AST_NODE *func)
{

    char *name = func->op.funcDecl.name;
    ast_list *body = func->op.funcDecl.body;
    char *returnType = func->op.funcDecl.returnType;
    ast_list *params = func->op.funcDecl.params;

    set(table->table, name, returnType);

    SymbolTable *funcScope = allocateNode(HASH_SIZE, name, table);

    ast_list *cursor = params;

    while (cursor != NULL)
    {

        AST_NODE *param = cursor->elem;

        set(funcScope->table, param->op.variableDecl.identifier, param->op.variableDecl.type);

        cursor = cursor->next;
    }

    cursor = body;

    while (cursor != NULL)
    {
        handleBlock(funcScope, cursor->elem);
        cursor = cursor->next;
    }
}

int addChild(SymbolTable *parent, SymbolTable *child)
{

    SymbolTable *cursor = parent->children;

    if (cursor == NULL)
    {
        parent->children = child;
        parent->next = NULL;
        return 0;
    }

    while (cursor->next != NULL)
    {
        cursor = cursor->next;
    }

    cursor->next = child;

    return 0;
}

SymbolTable *allocateNode(size_t hashSize, char *scope, SymbolTable *parent)
{
    SymbolTable *new = malloc(sizeof(SymbolTable));

    new->parent = parent;
    new->scope = scope;
    new->next = NULL;
    new->children = NULL;
    new->table = allocate(hashSize);

    if (parent != NULL)
    {
        addChild(parent, new);
    }

    return new;
}

SymbolTable *buildSymbolTable(AST_NODE *ast)
{

    SymbolTable *table = allocateNode(HASH_SIZE, "global", NULL);

    ast_list *programBody = ast->op.programDecl.body;

    ast_list *cursor = programBody;

    while (cursor != NULL)
    {

        AST_NODE *node = cursor->elem;

        switch (node->tag)
        {
        case function_decl:
        {
            handleFuncDecl(table, node);
            break;
        }
        }

        cursor = cursor->next;
    }

    return table;
}