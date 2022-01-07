#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include <ctype.h>


int in(char ** arr, int len, char * target){
  int i;
  for(i = 0; i < len; i++) {
    if(strncmp(arr[i], target, strlen(target)) == 0) {
      return 1;
    }
  }
  return 0;
}

int handleNumber(Lexic * lexic, char * source, int currIndex, int maxSize){

    int startIdx = currIndex;

    size_t numLength = 0;

    while(currIndex + 1 < maxSize && isdigit(source[currIndex + 1])){
        numLength += 1;
        currIndex++;
    }


    lexic->token = NUMBER;
    memcpy(lexic->lexeme, source + startIdx, numLength + 1);

    return numLength;

}

int handleIdentifier(Lexic * lexic, char * source, int currIndex, int maxSize){
    int startIndex = currIndex;

    int length = 0;


    while (isalpha(peek(source, currIndex, maxSize))){
        currIndex++;
        length++;
    }

    memcpy(lexic, source + startIndex, length + 1);

    lexic->token = IDENTIFIER;
    return length;
}


char peek(char * source, int currIndex, int maxSize){
    if(currIndex + 1 > maxSize){
        return '\0';
    }

    return source[currIndex + 1];
}



int lookAhead(char * source, int currIndex, int maxSize, char match){
    if(currIndex + 1 > maxSize){
        return 0;
    }

    if (source[currIndex + 1] == match){
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]){

    // if(argc < 2){
    //     printf("Missing file to parse!");
    // }

    char * fileName = "tests/test.c";

    printf("file: %s\n", fileName);

    FILE * source = fopen(fileName, "r");

    if(!source){
        printf("Failed to open file!\n");
        exit(0);
    }

    fseek(source, 0, SEEK_END);

    long fileSize = ftell(source);

    fseek(source, 0, SEEK_SET);

    char * sourceString = malloc(fileSize + 1);

    fread(sourceString, fileSize, 1, source);

    fclose(source);

    sourceString[fileSize] = '\0';

    Lexic * lexics = calloc(256, sizeof(Lexic));

    size_t size = 0;

    for(int i = 0; i < fileSize; i++){

        char character = sourceString[i];

        Lexic * lexic = malloc(sizeof(Lexic));
        memset(lexic, 0, sizeof(Lexic));

        switch(sourceString[i]){
            case '\t':
            case '\n':
            case ' ':
            {
               // fileSize--;
                continue;
            }
            case '(':{
                memcpy(lexic->lexeme, &character, 1);
                lexic->token = LEFT_PARENTHESIS;
                break;
            }
            case ')':{
                memcpy(lexic->lexeme, &character, 1);
                lexic->token = LEFT_PARENTHESIS;
                break;
            }
            case '{':{
                memcpy(lexic->lexeme, &character, 1);
                lexic->token = RIGHT_BRACKET;
                break;
            }
            case '}':{
                memcpy(lexic->lexeme, &character, 1);
                lexic->token = LEFT_BRACKET;
                break;
            }
            case ',': {
                memcpy(lexic->lexeme, &character, 1);
                lexic->token = COMMA;
                break;
            }
            case ';': {
                memcpy(lexic->lexeme, &character, 1);
                lexic->token = EOL;
                break;
            }
            case '=': {

                if(i += lookAhead(sourceString, i, fileSize, '=')){
                    memcpy(lexic->lexeme, sourceString + i, 2);
                    lexic->token = BINOP;
                    break;
                }

                memcpy(lexic->lexeme, &character, 1);
                lexic->token = EQUAL;
                break;
            }
            case '!':{
                if(i += lookAhead(sourceString, i, fileSize, '=')){
                    memcpy(lexic->lexeme, sourceString + i, 2);
                    lexic->token = BINOP;
                }

                printf("Missing equal");
                exit(1);
                break;
            }
            case '<':
            case '>':{
                if(i += lookAhead(sourceString, i, fileSize, '=')){
                    memcpy(lexic->lexeme, sourceString + i, 2);
                    lexic->token = BINOP;
                    break;
                }
                
                memcpy(lexic->lexeme, &character, 1);
                lexic->token = character == '>' ? GREATER_THAN : LESS_THAN;
                break;
            }
            case '+':
            case '-':{
                memcpy(lexic->lexeme, &character, 1);
                lexic->token = BINOP; 
            }
            case '"':{
                break;
            }
            default:{

                if(isdigit(character)){
                    i += handleNumber(lexic, sourceString, i, fileSize);
                }else if (isalpha(character)){
                    i += handleIdentifier(lexic, sourceString,i, fileSize);
                }else{
                    printf("Unexpected character %c\n", character);
                    exit(1);
                }
            }
        }

        lexics[size++] = *lexic;
    }


    for(int i = 0; i < size; i++){

        printf("=====\n");
        printf("lexeme %s\n", lexics[i].lexeme);
        printf("token %d\n", lexics[i].token);
        printf("=====\n");

    }



    free(sourceString);
    free(lexics);


    return 0;

}