#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

HashTable * allocate(int size){

    HashTable * table = malloc(sizeof(HashTable));

    table->backing = calloc(size, sizeof(Entry *));
    table->size = size;

    for(int i = 0; i < size; i++){
        table->backing[i] = malloc(sizeof(Entry));
        table->backing[i]->empty = 1;
        table->backing[i]->next = NULL;
    }

    return table;
}

// hash func from https://stackoverflow.com/questions/34595/what-is-a-good-hash-function;

int hash(HashTable * table, char * key){
    unsigned h = 0x811c9dc5;
    int i;

    for ( i = 0; i < strlen(key); i++ )
      h = ( h ^ key[i] ) * 0x01000193;

   return h % table->size;
}

int set(HashTable * table, char* key, char * type){

    int index = hash(table, key);

    Entry * newEntry = malloc(sizeof(Entry));

    newEntry->identifier = key;
    newEntry->type = type;
    newEntry->empty = 0;

    if(table->backing[index]->empty){
        table->backing[index] = newEntry;
        return 0;
    }

    Entry * cursor = table->backing[index];

    while(cursor->next != NULL){
        cursor = cursor->next;
    }

    cursor->next = newEntry;

    return 0;
}

int rem(HashTable * table,char * key){

    int index = hash(table, key);

    Entry * entry = table->backing[index];

    if(strcmp(entry->identifier, key) == 0){

        Entry * next = entry->next;

        free(entry);

        if(next != NULL){
            table->backing[index] = next;
            return 0;
        }

        return 0;
    }


    Entry * cursor = entry;
    Entry * prev = NULL;


    while(cursor != NULL){
        
        if(strcmp(cursor->identifier, key) == 0){

            if(cursor->next == NULL){ // last item in the list
                prev->next = NULL;
                free(cursor);
            }else if (cursor->next != NULL){ // in the middle of the list
                prev->next = cursor->next;
                free(cursor);
            }

            return 0;
        }

        prev = cursor;
        cursor = cursor->next;
    }
    
    return 0;

}

Entry * get(HashTable * table, char * key){

    int index = hash(table, key);

    Entry * entry = table->backing[index];


    if(entry->empty){
        return NULL;
    }

    if(strcmp(entry->identifier, key) == 0){
        return entry;
    }

    Entry * cursor = entry;

    while(cursor != NULL){
        
        if(strcmp(cursor->identifier, key) == 0){
            return cursor;
        }

        cursor = cursor->next;
    }
    
    return NULL;
}

int has(HashTable * table, char * key){
    if(get(table, key) != NULL){
        return 1;
    }

    return 0;
}

void clearTable(HashTable * table){

    for(int i = 0; i < table->size; i++){
        free(table->backing[i]);
    }

    free(table->backing);
    free(table);
}

