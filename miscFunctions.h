//
// Created by theo on 11/3/20.
//

#ifndef SYSPRO_1_MISCFUNCTIONS_H
#define SYSPRO_1_MISCFUNCTIONS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "structs.h"

#define MAX_STRING_LEN 20


int openFile(FILE ** fp, char * fileName);
int getArguments(int argc, char ** argv, int * diseaseHashSize, int * countryHashSize, int * bucketSize, char ** fileName);
void removeNewLine(char * buffer);
int memErrCheck(void * pointr);
int initRecordBuffer(patientRecord * buffer);
void freeRecordBuffer(patientRecord * buffer);
void getDate(date * dest, char * strDate);
int readLine(char * line, patientRecord * buffer);
void copyDate(date * dest, date const * src);
int cmpDates(date const * d1, date const * d2);

#endif //SYSPRO_1_MISCFUNCTIONS_H
