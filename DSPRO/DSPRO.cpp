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

unsigned long hashFunction(const char* str) {
    unsigned long hash = 5381;
    int currentChar;
    while ((currentChar = *str++))
        hash = ((hash << 5) + hash) + currentChar; 
    return hash % hashTableSize;
}
HashTable* createHashTable() {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable)* hashTableSize);
    if (table == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    for (int i = 0; i < hashTableSize; i++) {
        table[i].root = NULL;
    }
    return table;
}

Parcel* createParcel(const char* country, int weight, float value) {
    Parcel* newParcel = (Parcel*)malloc(sizeof(Parcel));
    if (newParcel == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strncpy(newParcel->country, country, maximumCountryLength);
    newParcel->country[maximumCountryLength] = '\0';
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
// saloni trying to add function which i missed suring writing the code.
int isCountryInTree(Parcel* root, const char* country) {
    if (root == NULL) {
        return 0;
    }
    if (strcmp(root->country, country) == 0) {
        return 1;
    }
    return isCountryInTree(root->left, country) || isCountryInTree(root->right, country);
}

int isCountryInHashTable(HashTable* table, const char* country) {
    unsigned long hashIndex = hashFunction(country);
    return isCountryInTree(table[hashIndex].root, country);
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

void displayParcelsByWeight(Parcel* root, int weight, int showGreaterThan) {
    if (showGreaterThan && root != NULL) {
        displayParcelsByWeight(root->left, weight, showGreaterThan);
        if (showGreaterThan && root->weight > weight) {
            printf("Country: %s, Weight: %d, Value: %.2f\n", root->country, root->weight, root->value);
        }
        else if (!showGreaterThan && root->weight < weight) {
            printf("Country: %s, Weight: %d, Value: %.2f\n", root->country, root->weight, root->value);
        }
        displayParcelsByWeight(root->right, weight,showGreaterThan);
    }
}


void displayParcelsForCountryByWeight(HashTable* table, const char* country, int weight,) {
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

void displayTotalLoadAndValuation(HashTable* table, const char* country) {
    if (!isCountryInHashTable(table, country)) {
        printf("No parcels found for %s.\n", country);
        return;
    }
    unsigned long hashIndex = hashFunction(country);
    printf("Displaying total load and valuation for country: %s\n", country);
    int totalLoad = 0;
    float totalValuation = 0.0;
    calculateTotalLoadAndValuation(table[hashIndex].root, &totalLoad, &totalValuation);
    printf("Total load for %s: %d grams\n", country, totalLoad);
    printf("Total valuation for %s: %.2f dollars\n", country, totalValuation);
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


void freeTree(Parcel * root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

void freeHashTable(HashTable* table) {
    for (int i = 0; i < hashTableSize; i++) {
        freeTree(table[i].root);
    }
    free(table);
}


void loadData(HashTable* table, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    char line[256];
    char country[maximumCountryLength + 1];
    int weight;
    float value;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%20[^,], %d, %f", country, &weight, &value) == 3) {
            insertParcel(table, country, weight, value);
        }
    }

    fclose(file);
}

void displayMenu() {
    printf("Menu:\n");
    printf("1. Enter country name and display all parcels\n");
    printf("2. Enter country and weight pair to display parcels\n");
    printf("3. Display total parcel load and valuation for a country\n");
    printf("4. Display cheapest and most expensive parcels for a country\n");
    printf("5. Display lightest and heaviest parcels for a country\n");
    printf("6. Exit\n");
}

int main() {
    HashTable* table = createHashTable();
    loadData(table, "couriers.txt");

    int choice;
    char country[maximumCountryLength + 1];
    int weight;

    do {
        displayMenu();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            exit(1);
        }
        getchar();

        switch (choice) {
        case 1:
            printf("Enter country name: ");
            scanf("%s", country);
            displayParcelsForCountry(table, country);
            break;
        case 2:
            printf("Enter country name: ");
            scanf("%s", country);
            printf("Enter weight: ");
            if (scanf("%d", &weight) != 1) {
                printf("Invalid input.\n");
                exit(1);
            }
            displayParcelsForCountryByWeight(table, country, weight);
            break;
        case 3:
            printf("Enter country name: ");
            scanf("%s", country);
            displayTotalLoadAndValuation(table, country);
            break;
        case 4:
            printf("Enter country name: ");
            scanf("%s", country);
            displayCheapestAndMostExpensiveParcels(table, country);
            break;
        case 5:
            printf("Enter country name: ");
            scanf("%s", country);
            displayLightestAndHeaviestParcels(table, country);
            break;
        case 6:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    freeHashTable(table);
    return 0;
}
//checking 