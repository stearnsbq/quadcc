#ifndef AST
#define AST

typedef struct AST_NODE AST_NODE;

struct AST_NODE
{
    enum
    {
        program_decl,
        if_stmt,
        function_decl,
        variable_decl,
        return_stmt,
        binary_exp,
        call_exp,
    } tag;

    union
    {

        struct AST_NODE *returnStatement;

        struct
        {
            char *type;
            char *identifier;
            union
            {
                int intValue;
                char charValue;
                struct callExp * func;
                char * variable;
            } init;
        } variableDecl;

        struct
        {
            AST_NODE *test;
            AST_NODE *consequent;
            AST_NODE *alternate;
        } ifStatement;

        struct
        {
            char *name;
            struct Exp_list *body;
        } programDecl;

        struct
        {
            char *operator;
            AST_NODE *left;
            AST_NODE *right;
        } binaryExp;

        struct
        {
            char *name;
            struct Exp_list *params;
            char *returnType;
            struct Exp_list *body;
        } funcDecl;

        struct
        {
            char *callee;
            struct Exp_list *arguments;
        } callExp;

    } op;
};

typedef struct Exp_list
{
    AST_NODE *elem;
    struct Exp_list *next;
} ast_list;

AST_NODE *make_variableDecl(char *type, char *identifier, AST_NODE * func, int intValue, char charValue, char * variable);
AST_NODE *make_IfStatement(AST_NODE *test, AST_NODE *consequent, AST_NODE *alternate);
AST_NODE *make_progamDecl(char *name, struct Exp_list *body);
AST_NODE *make_binaryExp(char *operator, AST_NODE * left, AST_NODE *right);
AST_NODE *make_funcDecl(char *name, struct Exp_list *params, char *returnType, struct Exp_list *body);
AST_NODE *make_callExp(char* callee, struct Exp_list *arguments);

#endif
