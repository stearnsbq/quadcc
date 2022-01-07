#include "lexic.h"

const char* KEYWORDS[] = {"int", "void", "return"};

const int KEYWORDS_LEN = 3;

int in(char ** arr, int len, char * target);

int handleNumber(Lexic * lexic, char * source, int currIndex, int maxSize);
char peek(char * source, int currIndex, int maxSize);
int handleIdentifier(Lexic * lexic, char * source, int currIndex, int maxSize); 
int lookAhead(char * source, int currIndex, int maxSize, char match);