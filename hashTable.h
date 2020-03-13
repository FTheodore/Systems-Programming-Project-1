//
// Created by theo on 13/3/20.
//

#ifndef SYSPRO_1_HASHTABLE_H
#define SYSPRO_1_HASHTABLE_H

#include <stdbool.h>
#include <math.h>
#include "miscFunctions.h"
#include "avlTree.h"

hashTable * initHashTable(int tableSize, int bucketSize);
listNode * initBucketNode(int maxEntriesInBucket);
int idToInt(char const * string);
int hashFunction(char const * string, int tableSize);
int insertToBucket(bucketNode * node, char * newString, date * newDate, listNode * recordPointer);
bool bucketEntryExists(char const * newString, listNode * head,listNode ** retAddress, int maxEntries, bool * noSpace);
int insertToHashTable(hashTable * hashT, char * newString, date * newDate, listNode * recordPointer);

#endif //SYSPRO_1_HASHTABLE_H
