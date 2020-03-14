//
// Created by theo on 10/3/20.
//

#ifndef SYSPRO_1_STRUCTS_H
#define SYSPRO_1_STRUCTS_H

typedef struct date {
    int day;
    int month;
    int year;
} date;

typedef struct patientRecord {
    char * patientId;
    char * firstName;
    char * lastName;
    char * diseaseId;
    char * country;
    date entryDate;
    date exitDate;
} patientRecord;

typedef struct listNode {
    void * dataPointer;
    struct listNode * next;
} listNode;

typedef struct hashTable {
    listNode ** table;
    int tableSize;
    int maxEntriesInBucket;
} hashTable;

typedef struct avlNode {
    struct avlNode * lChild;
    struct avlNode * rChild;
    int height;
    listNode * listHead;
    date nodeDate;
} avlNode;

typedef struct bucketEntry {
    avlNode * avlPtr;
    char * string;
} bucketEntry;

typedef struct bucketNode {
    bucketEntry * arrayOfEntries;
    int count;
} bucketNode;

typedef struct heapNode {
    struct heapNode * lChild;
    struct heapNode * rChild;
    int height;
    int count;
    char * string;
} heapNode;


#endif //SYSPRO_1_STRUCTS_H
