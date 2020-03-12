//
// Created by theo on 12/3/20.
//

#ifndef SYSPRO_1_LIST_H
#define SYSPRO_1_LIST_H


#include "stdbool.h"
#include "miscFunctions.h"


void copyDate(date * dest, date const * src);
patientRecord * initRecord(patientRecord buffer);
listNode * initPatientListNode(patientRecord buffer);
bool recordExists(char const * patientId, listNode * head, listNode ** retAddress);
bool checkInsertRecord(listNode ** head, patientRecord buffer, int * errno);
void freeData(char type, void ** ptr);
void freeList(char type, listNode ** head);

#endif //SYSPRO_1_LIST_H
