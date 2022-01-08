#ifndef LEXIC
#define LEXIC

typedef enum TokenType{
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    WHILE_KEYWORD,
    VOID_KEYWORD,
    RETURN_KEYWORD,
    IF_KEYWORD,
    EQUAL,
    COMMA,
    EOL,
    ENDOFFILE,
    VARTYPE,
    IDENTIFIER,
    BINOP, 
    NUMBER,
    CHARACTER,
    LESS_THAN,
    GREATER_THAN,
} TokenType;

typedef struct Lexic {
    char lexeme[32];
    TokenType token;
} Lexic;
#endif