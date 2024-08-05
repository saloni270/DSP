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