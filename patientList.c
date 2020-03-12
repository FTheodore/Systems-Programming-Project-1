//
// Created by theo on 12/3/20.
//

#include "patientList.h"

void copyDate(date * dest, date const * src) {
    dest->day = src->day;
    dest->month = src->month;
    dest->year = src->year;
}

patientRecord * initRecord(patientRecord buffer) {
    patientRecord * newRecord;

    newRecord = malloc(sizeof(patientRecord));
    if(newRecord == NULL) {
        printf("Error occurred during malloc\n");
        return NULL;
    }

    //copy patient's data from buffer
    strcpy(newRecord->country, buffer.country);
    strcpy(newRecord->diseaseId, buffer.diseaseId);
    strcpy(newRecord->firstName, buffer.firstName);
    strcpy(newRecord->lastName, buffer.lastName);
    strcpy(newRecord->patientId, buffer.patientId);
    copyDate(&newRecord->entryDate, &buffer.entryDate);
    copyDate(&newRecord->exitDate, &buffer.exitDate);

    return newRecord;
}

listNode * initRecordNode(patientRecord buffer) { // initialize a node for the list of patient records
    listNode * newNode;

    newNode = malloc(sizeof(patientRecord));
    if(newNode == NULL) {
        printf("Error occurred during malloc\n");
        return NULL;
    }

    newNode->next = NULL;
    newNode->dataPointer = initRecord(buffer);

    if(newNode->dataPointer == NULL)
        return NULL;

    return newNode;
}

/* bool recordExists(char const * patientId) {


} */