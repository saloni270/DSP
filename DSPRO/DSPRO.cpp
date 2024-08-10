#include <string.h>
#include <stdlib.h>
#include <stdio.h>


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

void insertParcel(HashTable* table, const char* country, int weight, float value) {
    unsigned long hashIndex = hashFunction(country);
    Parcel* newParcel = createParcel(country, weight, value);
    insertParcelToTree(&(table[hashIndex].root), newParcel);
    printf("Inserted: Country: %s, Weight: %d, Value: %.2f\n", country, weight, value);
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
 
void displayParcels(Parcel* root) {
    if (root != NULL) {
        displayParcels(root->left);
        printf("Country: %s, Weight: %d, Value: %.2f\n", root->country, root->weight, root->value);
        displayParcels(root->right);
    }
}

void displayParcelsForCountry(HashTable* table, const char* country) {
    if (!isCountryInHashTable(table, country)) {
        printf("No parcels found for %s.\n", country);
        return;
    }
    unsigned long hashIndex = hashFunction(country);
    printf("Displaying parcels for country: %s\n", country);
    printf("Parcels for %s (Hash Index: %lu):\n", country, hashIndex);
    displayParcels(table[hashIndex].root);
}

void displayParcelsByWeight(Parcel* root, int weight) {
    if (root != NULL) {
        displayParcelsByWeight(root->left, weight);
        if (root->weight > weight) {
            printf("Country: %s, Weight: %d, Value: %.2f\n", root->country, root->weight, root->value);
        }
        displayParcelsByWeight(root->right, weight);
    }
}

void displayParcelsForCountryByWeight(HashTable* table, const char* country, int weight) {
    if (!isCountryInHashTable(table, country)) {
        printf("No parcels found for %s.\n", country);
        return;
    }
    unsigned long hashIndex = hashFunction(country);
    printf("Displaying parcels for country: %s\n", country);
    printf("Parcels for %s with weight greater than %d (Hash Index: %lu):\n", country, weight, hashIndex);
    displayParcelsByWeight(table[hashIndex].root, weight);
} 
void calculateTotalLoadAndValuation(Parcel* root, int* totalLoad, float* totalValuation) {
    if (root != NULL) {
        calculateTotalLoadAndValuation(root->left, totalLoad, totalValuation);
        *totalLoad += root->weight;
        *totalValuation += root->value;
        calculateTotalLoadAndValuation(root->right, totalLoad, totalValuation);
    }
}

