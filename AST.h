#ifndef AST
#define AST

typedef struct AST_NODE AST_NODE;

struct AST_NODE {
    void * data;
    int type;
    AST_NODE * right;
    AST_NODE * left;
} ;
#endif


