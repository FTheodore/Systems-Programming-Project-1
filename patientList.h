//
// Created by theo on 12/3/20.
//

#ifndef SYSPRO_1_PATIENTLIST_H
#define SYSPRO_1_PATIENTLIST_H


#include <stdbool.h>
#include "miscFunctions.h"


patientRecord * initRecord(patientRecord buffer);
listNode * initPatientListNode(patientRecord buffer);
bool recordExists(char const * patientId, listNode * head, listNode ** retAddress);
bool checkInsertRecord(listNode ** head, patientRecord buffer, int * errNo, listNode ** retAddress);
void freePatientData(void ** ptr);
void freeRecordsList(listNode ** head);
void printRecordsList(listNode * head);

#endif //SYSPRO_1_PATIENTLIST_H
