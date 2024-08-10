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

Parcel* findCheapestParcel(Parcel* root) {
    if (root == NULL) {
        return NULL;
    }
    Parcel* cheapest = root;
    Parcel* leftCheapest = findCheapestParcel(root->left);
    Parcel* rightCheapest = findCheapestParcel(root->right);
    if (leftCheapest != NULL && leftCheapest->value < cheapest->value) {
        cheapest = leftCheapest;
    }
    if (rightCheapest != NULL && rightCheapest->value < cheapest->value) {
        cheapest = rightCheapest;
    }
    return cheapest;
}

Parcel* findMostExpensiveParcel(Parcel* root) {
    if (root == NULL) {
        return NULL;
    }
    Parcel* mostExpensive = root;
    Parcel* leftMostExpensive = findMostExpensiveParcel(root->left);
    Parcel* rightMostExpensive = findMostExpensiveParcel(root->right);
    if (leftMostExpensive != NULL && leftMostExpensive->value > mostExpensive->value) {
        mostExpensive = leftMostExpensive;
    }
    if (rightMostExpensive != NULL && rightMostExpensive->value > mostExpensive->value) {
        mostExpensive = rightMostExpensive;
    }
    return mostExpensive;
}

void displayCheapestAndMostExpensiveParcels(HashTable* table, const char* country) {
    if (!isCountryInHashTable(table, country)) {
        printf("No parcels found for %s.\n", country);
        return;
    }
    unsigned long hashIndex = hashFunction(country);
    printf("Displaying cheapest and most expensive parcels for country: %s\n", country);
    Parcel* cheapest = findCheapestParcel(table[hashIndex].root);
    Parcel* mostExpensive = findMostExpensiveParcel(table[hashIndex].root);
    if (cheapest != NULL) {
        printf("Cheapest parcel for %s: Weight: %d, Value: %.2f\n", country, cheapest->weight, cheapest->value);
    }
    if (mostExpensive != NULL) {
        printf("Most expensive parcel for %s: Weight: %d, Value: %.2f\n", country, mostExpensive->weight, mostExpensive->value);
    }
}


Parcel* findLightestParcel(Parcel* root) {
    if (root == NULL) {
        return NULL;
    }
    Parcel* lightest = root;
    Parcel* leftLightest = findLightestParcel(root->left);
    Parcel* rightLightest = findLightestParcel(root->right);
    if (leftLightest != NULL && leftLightest->weight < lightest->weight) {
        lightest = leftLightest;
    }
    if (rightLightest != NULL && rightLightest->weight < lightest->weight) {
        lightest = rightLightest;
    }
    return lightest;
}

Parcel* findHeaviestParcel(Parcel* root) {
    if (root == NULL) {
        return NULL;
    }
    Parcel* heaviest = root;
    Parcel* leftHeaviest = findHeaviestParcel(root->left);
    Parcel* rightHeaviest = findHeaviestParcel(root->right);
    if (leftHeaviest != NULL && leftHeaviest->weight > heaviest->weight) {
        heaviest = leftHeaviest;
    }
    if (rightHeaviest != NULL && rightHeaviest->weight > heaviest->weight) {
        heaviest = rightHeaviest;
    }
    return heaviest;
}

void displayLightestAndHeaviestParcels(HashTable* table, const char* country) {
    unsigned long hashIndex = hashFunction(country);
    if (table[hashIndex].root == NULL) {
        printf("No parcels found for %s.\n", country);
    }
    else {
        Parcel* lightest = findLightestParcel(table[hashIndex].root);
        Parcel* heaviest = findHeaviestParcel(table[hashIndex].root);
        if (lightest != NULL) {
            printf("Lightest parcel for %s: Weight: %d, Value: %.2f\n", country, lightest->weight, lightest->value);
        }
        if (heaviest != NULL) {
            printf("Heaviest parcel for %s: Weight: %d, Value: %.2f\n", country, heaviest->weight, heaviest->value);
        }
    }
}
