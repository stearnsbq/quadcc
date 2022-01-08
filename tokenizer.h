#ifndef TOKENIZER
#define TOKENIZER
#include "lexic.h"

typedef struct item_t
{
    const char *name;
    int value;
} item_t;

int lookup(const char *name);
int in(char **arr, int len, char *target);
int handleNumber(Lexic *lexic, char *source, int currIndex, int maxSize);
char peek(char *source, int currIndex, int maxSize);
int handleIdentifier(Lexic *lexic, char *source, int currIndex, int maxSize);
int lookAhead(char *source, int currIndex, int maxSize, char match);
#endif