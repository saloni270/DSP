#include <string.h>


#define hashTableSize 127
#define maximumCountryLength 20

typedef struct Parcel {
    char country[maximumCountryLength + 1];
    int weight;
    float value;
    struct Parcel* left;
    struct Parcel* right;
} Parcel;

typedef struct {
    Parcel* root;
} HashTable;

//tRYING TO FETCH IN HETVI





unsigned long hashFunction(const char* str) {
    unsigned long hash = 5381;
    int currentChar;
    while ((currentChar = *str++))
        hash = ((hash << 5) + hash) + currentChar; 
    return hash % hashTableSize;
}
HashTable* createHashTable() {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (table == NULL) {
        return NULL;  
    }
    table->root = (Parcel*)malloc(hashTableSize * sizeof(Parcel));
    if (table->root == NULL) {
        free(table);  
        return NULL;  
    }
    for (int i = 0; i < hashTableSize i++) {
        table->root[i] = NULL;
    }
    return table;
}

