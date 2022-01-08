#ifndef AST
#define AST

typedef struct AST_NODE AST_NODE;

struct AST_NODE
{
    enum
    {
        function_decl,
        integer_exp,
        variable_exp,
        binary_exp,
        call_exp
    } tag;

    union
    {

        int integerExp;

        char *variableExp;

        struct
        {
            char *operator;
            AST_NODE *left;
            AST_NODE *right;
        } binaryExp;

        struct
        {
            char *name;
            struct Exp_list *arguments;
        } funcDecl;

        struct
        {
            char *name;
            struct Exp_list *arguments;
        } callExp;

    } op;
};

typedef struct Exp_list
{
    AST_NODE *elem;
    struct Exp_list *next;
} ast_list;

#endif
