typedef struct Entry{
    char * identifier;
    char * type;
    int empty;
    struct Entry * next;
} Entry;


typedef struct HashTable{
    Entry ** backing;
    int size;
} HashTable;


HashTable * allocate(int size);

int hash(HashTable * table, char * key);

int set(HashTable * table, char* key, char * type);

int rem(HashTable * table, char * key);

Entry * get(HashTable * table, char * key);

int has(HashTable * table, char * key);

void clearTable(HashTable * table);