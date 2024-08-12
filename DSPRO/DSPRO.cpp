#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#pragma warning(disable: 4996)


#define hashTableSize 127
#define maximumCountryLength 20


// Defining the Parcel struct to hold parcel information.
typedef struct Parcel {
    char* country; // Dynamically allocated string for the country name.
    int weight;
    float value;
    struct Parcel* left;// according to BTS left has lesser value.
    struct Parcel* right;// according to BTS right has greater value.
} Parcel;

//structure of HashTable to hold an array of BST roots.
typedef struct {
    Parcel* root; // pointer to the root of a BST.
} HashTable;

// DJB2 hash function for generating hash values.
unsigned long hashFunction(const char* str) {
    unsigned long hash = 5381;
    int currentChar;
    while ((currentChar = *str++))
        hash = ((hash << 5) + hash) + currentChar; // hash * 33 + currentChar
    return hash % hashTableSize;
}
HashTable* createHashTable() {
    // Allocating the memory for the hash table, which is an array of `HashTable` structures.
    HashTable* table = (HashTable*)malloc(sizeof(HashTable)* hashTableSize);
    if (table == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    // Checking  an empty BST at the each hash index, and also intializing each bucket root to Null
    for (int i = 0; i < hashTableSize; i++) {
        table[i].root = NULL;
    }
    return table; // returing table (pointer ) to created the hash table.
}

Parcel* createParcel(const char* country, int weight, float value) {
    Parcel* newParcel = (Parcel*)malloc(sizeof(Parcel));
    if (newParcel == NULL) {
        printf ("Memory allocation failed.\n");
        exit(1);
    }
    // just copying the country name to the newly allocated memory.
    strncpy(newParcel->country, country, maximumCountryLength);
    newParcel->country[maximumCountryLength] = '\0';// terminating the string.
    newParcel->weight = weight;
    newParcel->value = value;
    newParcel->left = NULL;
    newParcel->right = NULL;
    return newParcel;// Returning the pointer to the newly created parcel.
}
void insertParcelToTree(Parcel** root, Parcel* newParcel) {
    if (*root == NULL) {// placing the new parcel there, if the current node is Null.
        *root = newParcel;
    }
    // inserting into the left sub-tree, if the new parcel's weight is less than the current node weight.
    else if (newParcel->weight < (*root)->weight) {
        insertParcelToTree(&((*root)->left), newParcel);// inserting into left-subtree, recursively.
    }
    // inserting into the left sub-tree, if the new parcel's weight is greater than the current node weight.
    else {
        insertParcelToTree(&((*root)->right), newParcel);// inserting into right-subtree, recursively.
    }
}
void insertParcel(HashTable* table, const char* country, int weight, float value) {
    unsigned long hashIndex = hashFunction(country);// calculating the hash-table
    Parcel* newParcel = createParcel(country, weight, value);
    insertParcelToTree(&(table[hashIndex].root), newParcel);// inserting new parcel to calculated hash index.

}
// country is not found, if the current node is Null.
int isCountryInTree(Parcel* root, const char* country) {
    if (root == NULL) {
        return 0;
    }
    // if the country is found it will return 1 as a true.
    if (strcmp(root->country, country) == 0) {
        return 1;
    }
    // searching for the left or right sub-tree.
    return isCountryInTree(root->left, country) || isCountryInTree(root->right, country);
}

int isCountryInHashTable(HashTable* table, const char* country) {
    unsigned long hashIndex = hashFunction(country);// calculating the hash-table
    return isCountryInTree(table[hashIndex].root, country);// if country exits, it calculated the hash index.
}
 
void displayParcels(Parcel* root) {
    if (root != NULL) { // proceed with traversal if the current node is not Null.
        displayParcels(root->left);
        printf("Country: %s, Weight: %d, Value: %.2f\n", root->country, root->weight, root->value);
        displayParcels(root->right);
    }
}

void displayParcelsForCountry(HashTable* table, const char* country) {
    if (!isCountryInHashTable(table, country)) {// checks if the country exists in the hash table.
        printf("No parcels found for %s.\n", country);
        return;
    }
    unsigned long hashIndex = hashFunction(country);// calculating the hash index for the country.
    printf("Displaying parcels for country: %s\n", country);
    printf("Parcels for %s (Hash Index: %lu):\n", country, hashIndex);
    displayParcels(table[hashIndex].root);// displaying all parcels and calculates the hash index.
}
int hasParcelsByWeight(Parcel* root, int weight, int showGreaterThan) {
    if (root == NULL) {// there are no more parcel, if the current node is null.
        return 0;
    }
    // checking condition if the weight condition is matches or not.
    if (showGreaterThan && root->weight > weight) {
        return 1; // returning true if does.
    }
    else if (!showGreaterThan && root->weight < weight) {
        return 1; // returning true if it does.
    }

    return hasParcelsByWeight(root->left, weight, showGreaterThan) ||  hasParcelsByWeight(root->right, weight, showGreaterThan);
}

void displayParcelsByWeight(Parcel* root, int weight, int showGreaterThan) {
    if (root != NULL) {// go with traversal if the current node is null.
        displayParcelsByWeight(root->left, weight, showGreaterThan);
        if (showGreaterThan && root->weight > weight) {
            printf("Country: %s, Weight: %d, Value: %.2f\n", root->country, root->weight, root->value);
        }
        else if (!showGreaterThan && root->weight < weight) {
            printf("Country: %s, Weight: %d, Value: %.2f\n", root->country, root->weight, root->value);
        }

        displayParcelsByWeight(root->right, weight, showGreaterThan);// displaying the parcels in the right sub-tree.
    }
}

void displayParcelsForCountryByWeight(HashTable* table, const char* country, int weight) {
    if (!isCountryInHashTable(table, country)) {// Check if the country exists in the hash table.
        printf("No parcels found for %s.\n", country);
        return;
    }
    unsigned long hashIndex = hashFunction(country);

    printf("Parcels for %s with weight less than %d:\n", country, weight); // informaning the user about weight creteria.
    if (!hasParcelsByWeight(table[hashIndex].root, weight, 0)) {
        printf("None\n");
    }
    else {
        displayParcelsByWeight(table[hashIndex].root, weight, 0);
    }

    printf("Parcels for %s with weight greater than %d:\n", country, weight);// informaning the user about weight creteria.
    if (!hasParcelsByWeight(table[hashIndex].root, weight, 1)) {
        printf("None\n");
    }
    else {
        displayParcelsByWeight(table[hashIndex].root, weight, 1);
    }
}

void calculateTotalLoadAndValuation(Parcel* root, int* totalLoad, float* totalValuation) {
    if (root != NULL) {
        calculateTotalLoadAndValuation(root->left, totalLoad, totalValuation);// processing with the calculation, if the node is null.
        *totalLoad += root->weight;// adding the current parcel's weigth to the total load.
        *totalValuation += root->value;// adding the current parcel's value to the total valuation.
        calculateTotalLoadAndValuation(root->right, totalLoad, totalValuation);
    }
}
void displayTotalLoadAndValuation(HashTable* table, const char* country) {
    if (!isCountryInHashTable(table, country)) { // Checking if the country exists in the hash table.
        printf("No parcels found for %s.\n", country);
        return;
    }
    unsigned long hashIndex = hashFunction(country);
    printf("Displaying total load and valuation for country: %s\n", country);
    int totalLoad = 0;
    float totalValuation = 0.0;
    calculateTotalLoadAndValuation(table[hashIndex].root, &totalLoad, &totalValuation);// Calculating the total load and valuation.
    printf("Total load for %s: %d grams\n", country, totalLoad);
    printf("Total valuation for %s: %.2f dollars\n", country, totalValuation);
}

Parcel* findCheapestParcel(Parcel* root) {
    if (root == NULL) {// if the current node is null return null and say no parcel found. 
        return NULL;
    }
    Parcel* cheapest = root;// assuming that chepeast is the current node.
    Parcel* leftCheapest = findCheapestParcel(root->left);// finding cheapest in the left sub-tree.
    Parcel* rightCheapest = findCheapestParcel(root->right);// finding the cheapeast in the right sub-rtree.
   // finding cheapeast from the both sub-tree.
    if (leftCheapest != NULL && leftCheapest->value < cheapest->value) {
        cheapest = leftCheapest;
    }
    if (rightCheapest != NULL && rightCheapest->value < cheapest->value) {
        cheapest = rightCheapest;
    }
    return cheapest;
}

Parcel* findMostExpensiveParcel(Parcel* root) {
    if (root == NULL) {// if the current node is null return null and say no parcel found.
        return NULL;
    }
    Parcel* mostExpensive = root;// assuming that most expensive is the current node
    Parcel* leftMostExpensive = findMostExpensiveParcel(root->left);// finding most expensive in the left sub - tree.
    Parcel* rightMostExpensive = findMostExpensiveParcel(root->right);// finding most expensive in the right sub - tree
    // finding the most expensive from the both sub-tree.
    if (leftMostExpensive != NULL && leftMostExpensive->value > mostExpensive->value) {
        mostExpensive = leftMostExpensive;
    }
    if (rightMostExpensive != NULL && rightMostExpensive->value > mostExpensive->value) {
        mostExpensive = rightMostExpensive;
    }
    return mostExpensive;
}

void displayCheapestAndMostExpensiveParcels(HashTable* table, const char* country) {
    if (!isCountryInHashTable(table, country)) { // Checking if the country exists in the hash table.
        printf("No parcels found for %s.\n", country);
        return;
    }
    unsigned long hashIndex = hashFunction(country);
    printf("Displaying cheapest and most expensive parcels for country: %s\n", country);
    Parcel* cheapest = findCheapestParcel(table[hashIndex].root);// calculating the hash index for the cheapeast parcel.
    Parcel* mostExpensive = findMostExpensiveParcel(table[hashIndex].root);// calculating the hash index for the most expensive parcel.
    if (cheapest != NULL) {
        printf("Cheapest parcel for %s: Weight: %d, Value: %.2f\n", country, cheapest->weight, cheapest->value);
    }
    if (mostExpensive != NULL) {
        printf("Most expensive parcel for %s: Weight: %d, Value: %.2f\n", country, mostExpensive->weight, mostExpensive->value);
    }
}


Parcel* findLightestParcel(Parcel* root) {
    if (root == NULL) {// if the current node is null return null and say no parcel found.
        return NULL;
    }
    Parcel* lightest = root;// assuming that lighest is the current node
    Parcel* leftLightest = findLightestParcel(root->left);// finding lighest in the left sub - tree
    Parcel* rightLightest = findLightestParcel(root->right);// finding lighest in the right sub - tree
    // finding the lighest from the both sub-tree.
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
    Parcel* heaviest = root;// assuming that lighest is the current node
    Parcel* leftHeaviest = findHeaviestParcel(root->left);// finding heaviest in the left sub - tree
    Parcel* rightHeaviest = findHeaviestParcel(root->right);// finding heaviest in the right sub - tree
    // finding the heaviest from the both sub-tree.
    if (leftHeaviest != NULL && leftHeaviest->weight > heaviest->weight) {
        heaviest = leftHeaviest;
    }
    if (rightHeaviest != NULL && rightHeaviest->weight > heaviest->weight) {
        heaviest = rightHeaviest;
    }
    return heaviest;
}

void displayLightestAndHeaviestParcels(HashTable* table, const char* country) {
    if (!isCountryInHashTable(table, country)) {// Checking if the country exists in the hash table.
        printf("No parcels found for %s.\n", country);
        return;
    }
    unsigned long hashIndex = hashFunction(country);
    printf("Displaying lightest and heaviest parcels for country: %s\n", country);// calculating the hash index for the both.
    Parcel* lightest = findLightestParcel(table[hashIndex].root);// calculating the hash index for the lighest parcel.
    Parcel* heaviest = findHeaviestParcel(table[hashIndex].root);// calculating the hash table for the heaviest parcel.
    if (lightest != NULL) {
        printf("Lightest parcel for %s: Weight: %d, Value: %.2f\n", country, lightest->weight, lightest->value);
    }
    if (heaviest != NULL) {
        printf("Heaviest parcel for %s: Weight: %d, Value: %.2f\n", country, heaviest->weight, heaviest->value);
    }
}



void freeTree(Parcel * root) {
    if (root != NULL) {// if the current node is not null, then proceed with the free uping the memory.
        freeTree(root->left);// free the left sub-tree
        freeTree(root->right);// free the right sub-tree
        free(root);// free the current root from the struct of the parcel.
    }
}

void freeHashTable(HashTable* table) {
    for (int i = 0; i < hashTableSize; i++) {// for going to each bucket in the has table.
        freeTree(table[i].root);// free the hash index from the BST.
    }
    free(table);// free the memory allocated hash table.
}

// loading the country name, weight and the value from txt file into the hash table.
void loadData(HashTable* table, const char* filename) {
    FILE* file = fopen(filename, "r");// function used to open the file in the read only mode.
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    char line[256];// storing the each line read data from the txt file.
    char country[maximumCountryLength + 1];
    int weight;// storing the value 
    float value;// storing the value

    // getting the information from the file and read the data line by line. 
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%20[^,], %d, %f", country, &weight, &value) == 3) {
            insertParcel(table, country, weight, value);// inserting the parsed information into the hash table.
        }
    }

    fclose(file); // function used to close the txt file after reading all the data.
}

// function to help the user to choose the menu from given choice.
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
    HashTable* table = createHashTable();// creating the hash table
    loadData(table, "couriers.txt");// loading the data from the file into hash table.

    int choice;
    char country[maximumCountryLength + 1];// help to store the country name input by the user.
    int weight;

    do {
        displayMenu();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {// getting the user choice.
            printf("Invalid input.\n");
            exit(1);
        }
        getchar();

        switch (choice) { // changing made in the output by the user choice.
        case 1:
            printf("Enter country name: ");
            fgets(country, sizeof(country), stdin);// remove newline character.
            country[strcspn(country, "\n")] = '\0'; 
            displayParcelsForCountry(table, country);
            break;

        case 2:
            printf("Enter country name: ");
            fgets(country, sizeof(country), stdin););// remove newline character.
            country[strcspn(country, "\n")] = '\0';
            printf("Enter weight: ");
            if (scanf("%d", &weight) != 1) {
                printf("Invalid input.\n");
                exit(1);
            }
            displayParcelsForCountryByWeight(table, country, weight);
            break;
        case 3:
            printf("Enter country name: ");
            fgets(country, sizeof(country), stdin););// remove newline character.
            country[strcspn(country, "\n")] = '\0';
            displayTotalLoadAndValuation(table, country);
            break;
        case 4:
            printf("Enter country name: ");
            fgets(country, sizeof(country), stdin););// remove newline character.
            country[strcspn(country, "\n")] = '\0';
            displayCheapestAndMostExpensiveParcels(table, country);
            break;
        case 5:
            printf("Enter country name: ");
            fgets(country, sizeof(country), stdin););// remove newline character.
            country[strcspn(country, "\n")] = '\0';
            displayLightestAndHeaviestParcels(table, country);
            break;
        case 6:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);// run until the user doesn't want to exit the code.

    freeHashTable(table);// free all the dynamically allocation memory 
    return 0;
}