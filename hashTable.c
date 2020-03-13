//
// Created by theo on 13/3/20.
//

#include "hashTable.h"

hashTable * initHashTable(int tableSize, int bucketSize) { // initialize a new hash table
    hashTable * newTable;

    newTable = malloc(sizeof(hashTable));
    if(memErrCheck(newTable))
        return NULL;

    newTable->tableSize = tableSize;
    newTable->maxEntriesInBucket = (bucketSize - sizeof(listNode *)) / sizeof(bucketEntry);
    // now allocate the actual array
    newTable->table = malloc(tableSize * sizeof(listNode *));
    if(memErrCheck(newTable->table))
        return NULL;

    for (int i = 0; i < tableSize; ++i) {
        newTable->table[i] = NULL;
    }

    return newTable;
}

listNode * initBucketNode(int maxEntriesInBucket) {
    listNode * newNode;

    newNode = malloc(sizeof(listNode));
    if(memErrCheck(newNode))
        return NULL;

    newNode->next = NULL;
    newNode->dataPointer = malloc(sizeof(bucketNode));
    if(memErrCheck(newNode->dataPointer))
        return NULL;

    ((bucketNode *)newNode->dataPointer)->arrayOfEntries = malloc(maxEntriesInBucket * sizeof(bucketEntry));
    if(memErrCheck(((bucketNode *)newNode->dataPointer)->arrayOfEntries))
        return NULL;

    for (int i = 0; i < maxEntriesInBucket; ++i) {
        ((bucketNode *)newNode->dataPointer)->arrayOfEntries->string = NULL;
        ((bucketNode *)newNode->dataPointer)->arrayOfEntries->ptr = NULL;
    }

    ((bucketNode *)newNode->dataPointer)->count = 0;

    return newNode;
}

int idToInt(char const * string) {
    int sum = 0;
    int j = 0;
    for(int i = ((int)strlen(string) - 1); i >= 0; --i) {
        sum += string[i] * pow(2, j);
        j++;
    }
    return sum;
}

int hashFunction(char const * string, int tableSize) {
    int key = idToInt(string);

    return (2 * key + 1) % tableSize;
}

bool bucketEntryExists(char const * newString, listNode * head,\
listNode ** retAddress, int maxEntries, bool * noSpace) {
    // check if the same entry already exists in bucket and return the appropriate memory address
    int entriesInNode = ((bucketNode *)head->dataPointer)->count;
    for (int i = 0; i < entriesInNode; ++i) {
        if(strcmp(((bucketNode *)head->dataPointer)->arrayOfEntries[i].string,newString) == 0) { // entry exists
            *retAddress = head;
            return true;
        }
    }

    if(head->next != NULL) {
        return bucketEntryExists(newString,head->next,retAddress,maxEntries,noSpace);
    }
    else { // reached end of bucket, entry not found
        *retAddress = head;

        if(entriesInNode == maxEntries) // inform whether current node has space for a new entry
            *noSpace = true;
        else
            *noSpace = false;

        return false;
    }

}

int insertToBucket(bucketNode * node, char * newString, date * newDate, listNode * recordPointer) {
    int retVal;

    for (int i = 0; i < node->count; ++i) {
        if(strcmp(newString,node->arrayOfEntries[i].string) == 0) {
            retVal = insertToAvlTree(&node->arrayOfEntries[i].ptr,newDate,recordPointer,node->arrayOfEntries[i].ptr);
            if(retVal)
                return -1;

            return 0;
        }
    }

    // new entry must be created
    node->arrayOfEntries[node->count].string = strdup(newString);
    retVal = insertToAvlTree(&node->arrayOfEntries[node->count].ptr,newDate,\
    recordPointer,node->arrayOfEntries[node->count].ptr);
    if(retVal)
        return -1;

    ++(node->count);
    return 0;
}

int insertToHashTable(hashTable * hashT, char * newString, date * newDate, listNode * recordPointer) {
    // first hash the given string
    int index = hashFunction(newString,hashT->tableSize);

    //current bucket is empty
    if(hashT->table[index] == NULL) {
        hashT->table[index] = initBucketNode(hashT->maxEntriesInBucket);
        if(hashT->table[index] == NULL)
            return -1;

        // insert the entry to the new bucket
        return insertToBucket(hashT->table[index]->dataPointer,newString,newDate,recordPointer);
    }

    listNode * nodeToInsert;
    bool noSpace;
    bool entryExists = bucketEntryExists(newString,hashT->table[index],\
    &nodeToInsert,hashT->maxEntriesInBucket,&noSpace);

    if(entryExists || !noSpace) {
        return insertToBucket(nodeToInsert->dataPointer,newString,newDate,recordPointer);
    }
    else { // need to create new entry and the bucket node is full
       nodeToInsert->next = initBucketNode(hashT->maxEntriesInBucket);
       if(nodeToInsert->next == NULL)
           return -1;

        return insertToBucket(nodeToInsert->next->dataPointer,newString,newDate,recordPointer);
    }
}