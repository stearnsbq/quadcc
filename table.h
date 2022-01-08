typedef struct Entry{
    char * identifier;
    int scope;
    char * type;
    int empty;
    struct Entry * next;
} Entry;


typedef struct SymbolTable{
    Entry ** backing;
    int size;
} SymbolTable;


SymbolTable * allocate(int size);

int hash(SymbolTable * table, char * key);

int set(SymbolTable * table, char* key, int scope, char * type);

int rem(SymbolTable * table, char * key);

Entry * get(SymbolTable * table, char * key);

void clearTable(SymbolTable * table);