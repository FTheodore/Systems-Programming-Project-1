//
// Created by theo on 13/3/20.
//

#ifndef SYSPRO_1_HASHTABLE_H
#define SYSPRO_1_HASHTABLE_H

#include <stdbool.h>
#include <math.h>
#include "miscFunctions.h"
#include "avlTree.h"
#include "binaryHeap.h"

hashTable * initHashTable(int tableSize, int bucketSize);
listNode * initBucketNode(int maxEntriesInBucket);
int idToInt(char const * string);
int hashFunction(char const * string, int tableSize);
int insertToBucket(bucketNode * node, char * newString, date * newDate, listNode * recordPointer);
bool bucketEntryExists(char const * newString, listNode * head,listNode ** retAddress, int maxEntries, bool * noSpace);
int insertToHashTable(hashTable * hashT, char * newString, date * newDate, listNode * recordPointer);
void freeBucketList(listNode ** head);
void freeHashTable(hashTable ** hashT);
void printHashList(listNode * head);
void printHashTable(hashTable * hashT);
void getAllDiseaseStats(hashTable * diseaseTbl, bool datesGiven, date start, date end);
void getDiseaseStats(hashTable * diseaseTbl, char * virusName, bool countryGiven, char *country, date start, date end);
int getEntryTopValues(hashTable * hashT,char * string,int numOfValues,char type,bool datesGiven,date start,date end);
void getDiseaseSickPatients(hashTable * diseaseTbl, char * virusName);
void getAllSickPatients(hashTable * diseaseTbl);

#endif //SYSPRO_1_HASHTABLE_H
