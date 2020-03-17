//
// Created by theo on 11/3/20.
//

#ifndef SYSPRO_1_MISCFUNCTIONS_H
#define SYSPRO_1_MISCFUNCTIONS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "structs.h"
#include <stdbool.h>
#include "patientList.h"
#include "avlTree.h"
#include "hashTable.h"

#define MAX_STRING_LEN 20
#define MAX_LINE_LEN 150
#define ARG_LEN 30


int openFile(FILE ** fp, char * fileName);

int getArguments(int argc, char ** argv, int * diseaseHashSize,\
int * countryHashSize, int * bucketSize, char ** fileName);

void removeNewLine(char * buffer);
int memErrCheck(void * pointr);
int initRecordBuffer(patientRecord * buffer);
void freeRecordBuffer(patientRecord * buffer);
void getDate(date * dest, char * strDate);
int readLine(char * line, patientRecord * buffer);
void copyDate(date * dest, date const * src);
int cmpDates(date const * d1, date const * d2);
void printRecord(listNode * patientRec);
int getInput(char * buff, int buffSize);
void getCommand(char const * buff, char * comm, int lim);
int getArgumentsNum(char const * buff);
void getNthArgument(char const * buff, char * argmnt, int n);
char * initStrBuffer(int bufferSize);
void getAndCopy(char * inputBuff,int n,char * dest);
int readInput(char * inputBuff, patientRecord * buffer);
bool datesCorrect(date * entryDate, date * exitDate);
int insertToStructs(listNode ** recordsListHead, patientRecord * recordBuffer, listNode ** recordPointer,\
hashTable * diseaseHashTbl, hashTable * countryHashTbl);
void getDates(char * inputBuff,int startDateInd,int endDateInd,date * start,date * end);
int processInputAndCallHeap(char * inputBuffer,char type,hashTable * hash);
bool noValue(date dt);

#endif //SYSPRO_1_MISCFUNCTIONS_H
