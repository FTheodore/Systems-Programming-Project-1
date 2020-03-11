//
// Created by theo on 10/3/20.
//

#ifndef SYSPRO_1_STRUCTS_H
#define SYSPRO_1_STRUCTS_H

typedef struct patientRecord {
    char * patientId;
    char * firstName;
    char * lastName;
    char * diseaseId;
    char * country;
    char * entryDate;
    char * exitDate;
} patientRecord;

typedef struct listNode {
    void * dataPointer;
    struct listNode * next;
} listNode;

typedef struct hashTable {
    listNode * array;
    int arraySize;
} hashTable;

typedef struct treeNode {
    //******//
} treeNode;

typedef struct bucketEntry {
    treeNode * ptr;
    char * string;
} bucketEntry;


#endif //SYSPRO_1_STRUCTS_H
