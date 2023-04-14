#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType *next; // create a next pointer (for linkedlist structure)
};

// Fill out this structure
struct HashType
{
    struct RecordType *record; // create a pointer to a RecordType
};

// Compute the hash function
int hash(int x, int tableSize)
{
    // hash function is x % tableSize
    return x % tableSize;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// insert record into the hash table
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize)
{
     // call the hash function to get the index
    int index = hash(record->id, tableSize);

    // if the RecordType at that index is NULL
    if (hashTable[index].record == NULL)
    {
        // set 'record' equal to the HashType pointer in the table at index
        hashTable[index].record = record;
    }
    else
    {
        // else traverse to the end of the linkedlist and add 'record' to the end of it
        struct RecordType *current = hashTable[index].record;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = record;
        current->next->next = NULL; // Set the next pointer of the last element to NULL
    }
}


// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; i++) {
        if (pHashArray[i].record != NULL) {
            printf("Index %d -> ", i);
            struct RecordType *current = pHashArray[i].record;
            while (current != NULL) {
                printf("%d,%c,%d", current->id, current->name, current->order);
                if (current->next != NULL) {
                    printf(" -> ");
                }
                current = current->next;
            }
            printf("\n");
        }
        else{
            printf("Index %d -> NULL\n", i);
        }
    }
}


int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);
    // use Calloc
    // Your hash implementation
    // Initialize the hash table
    int hashTableSize = 11; // can be any prime value
    struct HashType *hashTable = (struct HashType *)calloc(hashTableSize, sizeof(struct HashType));
    int i;
    for (i = 0; i < recordSz; ++i) {
        insertRecord(hashTable, &pRecords[i], hashTableSize);
    }
    // call the display records function
    
    displayRecordsInHash(hashTable, hashTableSize);
    // free all the allocated memory
    for (i = 0; i < hashTableSize; ++i) {
        free(hashTable[i].record);
    }
    free(hashTable);
    free(pRecords);
}