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
