//
// Created by theo on 12/3/20.
//

#include "patientList.h"

patientRecord * initRecord(patientRecord buffer) { // initialize a patient record
    patientRecord * newRecord;

    newRecord = malloc(sizeof(patientRecord));
    if(memErrCheck(newRecord))
        return NULL;

    //copy patient's data from buffer
    newRecord->country = strdup(buffer.country);
    if(memErrCheck(newRecord->country))
        return NULL;

    newRecord->patientId = strdup(buffer.patientId);
    if(memErrCheck(newRecord->patientId))
        return NULL;

    newRecord->lastName = strdup(buffer.lastName);
    if(memErrCheck(newRecord->lastName))
        return NULL;

    newRecord->firstName = strdup(buffer.firstName);
    if(memErrCheck(newRecord->firstName))
        return NULL;

    newRecord->diseaseId = strdup(buffer.diseaseId);
    if(memErrCheck(newRecord->diseaseId))
        return NULL;

    copyDate(&newRecord->entryDate, &buffer.entryDate);
    copyDate(&newRecord->exitDate, &buffer.exitDate);

    return newRecord;
}

listNode * initPatientListNode(patientRecord buffer) { // initialize a node for the list of patient records
    listNode * newNode;

    newNode = malloc(sizeof(listNode));
    if(memErrCheck(newNode))
        return NULL;

    newNode->next = NULL;
    newNode->dataPointer = initRecord(buffer);

    if(newNode->dataPointer == NULL)
        return NULL;

    return newNode;
}

bool recordExists(char const * patientId, listNode * head, listNode ** retAddress) {
    // check if record already exists and return the memory address of the last node in the list
    if(strcmp(patientId, ((patientRecord *)head->dataPointer)->patientId) == 0)
        return true;
    else if(head->next != NULL)
        return recordExists(patientId, head->next, retAddress);
    else {
        *retAddress = head;
        return false;
    }
}

bool checkInsertRecord(listNode ** head, patientRecord buffer, int * errNo, listNode ** retAddress) {
    if(*head == NULL) { // if list is empty insert the new node
        *head = initPatientListNode(buffer);
        if(*head == NULL) {
            *errNo = -1;
            return false;
        }
        *retAddress = *head;
        return true;
    }

    listNode * lastNode;
    bool retVal;

    // check if duplicate
    retVal = recordExists(buffer.patientId, *head, &lastNode);
    if(retVal == true) // cant insert record,it is a duplicate
        return false;

    listNode * newNode = initPatientListNode(buffer);
    if(newNode == NULL) {
        *errNo = -1;
        return false;
    }

    lastNode->next = newNode; // insert the new record at the end of the list
    *retAddress = newNode;
    return true;
}

void freeRecordsList(listNode ** head) {
    if(*head != NULL) {
        freeRecordsList(&(*head)->next);
        freePatientData((patientRecord **)&(*head)->dataPointer);
        free(*head);
    }
}

void freePatientData(patientRecord ** ptr) {
    free( (*ptr)->patientId );
    free( (*ptr)->diseaseId );
    free( (*ptr)->lastName );
    free( (*ptr)->firstName );
    free( (*ptr)->country );

    free(*ptr);
}

void printRecordsList(listNode * head) {
    if(head != NULL) {
        printRecord(head);
        printRecordsList(head->next);
    }
}

void updateExitDate(listNode * head, date newDate, char * patientId) {
    if(head != NULL) {
        patientRecord * recordPtr = head->dataPointer;
        if(strcmp(recordPtr->patientId,patientId)==0) {
            if(!datesCorrect(&recordPtr->entryDate,&newDate))
                printf("Couldn't update patient record\n");
            else
                copyDate(&recordPtr->exitDate,&newDate);

            return;
        }

        updateExitDate(head->next,newDate,patientId);
    }
    else { // no such patient exists
        printf("No patient with the id given was found\n");
    }
}