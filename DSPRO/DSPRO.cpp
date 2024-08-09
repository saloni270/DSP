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
// trying to fetching in saloni

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

Parcel* createParcel(const char* country, int weight, float value) {
    Parcel* newParcel = (Parcel*)malloc(sizeof(Parcel));
    if (newParcel == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    strncpy(newParcel->country, country, MAX_COUNTRY_NAME_LENGTH);
    newParcel->country[MAX_COUNTRY_NAME_LENGTH] = '\0';
    newParcel->weight = weight;
    newParcel->value = value;
    newParcel->left = NULL;
    newParcel->right = NULL;
    return newParcel;
}
void insertParcelToTree(Parcel** root, Parcel* newParcel) {
    if (*root == NULL) {
        *root = newParcel;
    }
    else if (newParcel->weight < (*root)->weight) {
        insertParcelToTree(&((*root)->left), newParcel);
    }
    else {
        insertParcelToTree(&((*root)->right), newParcel);
    }
}
