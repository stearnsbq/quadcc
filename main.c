#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include <ctype.h>
#include "parser.h"

int lookup(const char *name)
{
    item_t table[] = {
        {"int", VARTYPE},
        {"void", VOID_KEYWORD},
        {"return", RETURN_KEYWORD},
        {"if", IF_KEYWORD},
        {NULL, EOF}};

    for (item_t *p = table; p->name != NULL; ++p)
    {
        if (strcmp(p->name, name) == 0)
        {
            return p->value;
        }
    }
    return IDENTIFIER;
}

int handleNumber(Lexic *lexic, char *source, int currIndex, int maxSize)
{

    int startIdx = currIndex;

    size_t numLength = 0;

    while (currIndex + 1 < maxSize && isdigit(source[currIndex + 1]))
    {
        numLength += 1;
        currIndex++;
    }

    lexic->token = NUMBER;
    memcpy(lexic->lexeme, source + startIdx, numLength + 1);

    return numLength;
}

int handleIdentifier(Lexic *lexic, char *source, int currIndex, int maxSize)
{
    int startIndex = currIndex;

    int length = 0;

    while (isalpha(peek(source, currIndex, maxSize)))
    {
        currIndex++;
        length++;
    }

    memcpy(lexic->lexeme, source + startIndex, length + 1);

    lexic->token = lookup(lexic->lexeme);

    return length;
}

char peek(char *source, int currIndex, int maxSize)
{
    if (currIndex + 1 > maxSize)
    {
        return '\0';
    }

    return source[currIndex + 1];
}

int lookAhead(char *source, int currIndex, int maxSize, char match)
{
    if (currIndex + 1 > maxSize)
    {
        return 0;
    }

    if (source[currIndex + 1] == match)
    {
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{

    // if(argc < 2){
    //     printf("Missing file to parse!");
    // }

    char *fileName = "tests/fibonacci.c";

    printf("file: %s\n", fileName);

    FILE *source = fopen(fileName, "r");

    if (!source)
    {
        printf("Failed to open file!\n");
        exit(0);
    }

    fseek(source, 0, SEEK_END);

    long fileSize = ftell(source);

    fseek(source, 0, SEEK_SET);

    char *sourceString = malloc(fileSize + 1);

    fread(sourceString, fileSize, 1, source);

    fclose(source);

    sourceString[fileSize] = '\0';

    Lexic *lexics = calloc(512, sizeof(Lexic));

    size_t size = 0;

    for (int i = 0; i < fileSize; i++)
    {

        char character = sourceString[i];

        Lexic *lexic = malloc(sizeof(Lexic));
        memset(lexic, 0, sizeof(Lexic));

        switch (sourceString[i])
        {
        case '\t':
        case '\n':
        case ' ':
        {
            continue;
        }
        case '(':
        {
            memcpy(lexic->lexeme, &character, 1);
            lexic->token = LEFT_PARENTHESIS;
            break;
        }
        case ')':
        {
            memcpy(lexic->lexeme, &character, 1);
            lexic->token = RIGHT_PARENTHESIS;
            break;
        }
        case '{':
        {
            memcpy(lexic->lexeme, &character, 1);
            lexic->token = LEFT_BRACKET;
            break;
        }
        case '}':
        {
            memcpy(lexic->lexeme, &character, 1);
            lexic->token = RIGHT_BRACKET;
            break;
        }
        case ',':
        {
            memcpy(lexic->lexeme, &character, 1);
            lexic->token = COMMA;
            break;
        }
        case ';':
        {
            memcpy(lexic->lexeme, &character, 1);
            lexic->token = EOL;
            break;
        }
        case '=':
        {

            int look = lookAhead(sourceString, i, fileSize, '=');

            if (look)
            {  
                
                memcpy(lexic->lexeme, sourceString + i, 2);
                i += look; 
                lexic->token = BINOP;
                break;
            }

            memcpy(lexic->lexeme, &character, 1);
            lexic->token = EQUAL;
            break;
        }
        case '!':
        {
            int look = lookAhead(sourceString, i, fileSize, '=');

            if (look)
            {
                memcpy(lexic->lexeme, sourceString + i, 2);
                i+=look;
                lexic->token = BINOP;
            }

            printf("Missing equal");
            exit(1);
            break;
        }
        case '<':
        case '>':
        {
            int look = lookAhead(sourceString, i, fileSize, '=');

            if (look)
            {
                memcpy(lexic->lexeme, sourceString + i, 2);
                i+=look;
                lexic->token = BINOP;
                break;
            }

            memcpy(lexic->lexeme, &character, 1);
            lexic->token = character == '>' ? GREATER_THAN : LESS_THAN;
            break;
        }
        case '+':
        case '-':
        {
            memcpy(lexic->lexeme, &character, 1);
            lexic->token = BINOP;
            break;
        }
        case '&':{
            int look = lookAhead(sourceString, i, fileSize, '&');

            if (look)
            {
                memcpy(lexic->lexeme, sourceString + i, 2);
                i+=look;
                lexic->token = BINOP;
            }else{
                printf("missing &\n");
                exit(1);
            }

            break;

        }
        case '"':
        {
            break;
        }
        default:
        {

            if (isdigit(character))
            {
                i += handleNumber(lexic, sourceString, i, fileSize);
            }
            else if (isalpha(character))
            {
                i += handleIdentifier(lexic, sourceString, i, fileSize);
            }
            else
            {
                printf("Unexpected character %c\n", character);
                exit(1);
            }
        }
        }

        lexics[size++] = *lexic;
    }

    Lexic * endOfFile = malloc(sizeof(Lexic));
    endOfFile->token = ENDOFFILE;

    lexics[size++] = *endOfFile;

    for (int i = 0; i < size; i++)
    {

        printf("=====\n");
        printf("lexeme %s\n", lexics[i].lexeme);
        printf("token %d\n", lexics[i].token);
        printf("=====\n");
    }

    AST_NODE * ast = parse(lexics);

    free(sourceString);
    free(lexics);

    return 0;
}