//
// Created by theo on 11/3/20.
//

#include "miscFunctions.h"

int openFile(FILE ** fp, char * fileName) {
    char *absolutePath;
    absolutePath = malloc(strlen(fileName) + strlen("../") + 1);
    if(memErrCheck(absolutePath))
        return -1;

    strcpy(absolutePath,"../");
    strcat(absolutePath,fileName);

    if((*fp = fopen(absolutePath,"r")) == NULL) {
        printf("Error, couldn't open the records file\n");
        return -1;
    }
    free(absolutePath);
    return 0;
}

int getArguments(int argc, char ** argv, int * diseaseHashSize,\
int * countryHashSize, int * bucketSize, char ** fileName) {
    if(argc != 9) {
        printf("Wrong input\n");
        return -1;
    }

    int argsRead = 0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h1") == 0) {
            *diseaseHashSize = atoi(argv[i + 1]);
            ++argsRead;
            i += 1;
        } else if (strcmp(argv[i], "-h2") == 0) {
            *countryHashSize = atoi(argv[i + 1]);
            ++argsRead;
            i += 1;
        } else if (strcmp(argv[i], "-b") == 0) {
            *bucketSize = atoi(argv[i + 1]);
            ++argsRead;
            i += 1;
        } else if (strcmp(argv[i], "-p") == 0) {
            *fileName = malloc(strlen(argv[i + 1]) + 1);
            if(memErrCheck(*fileName))
                return -1;
            strcpy(*fileName, argv[i + 1]);
            ++argsRead;
            i += 1;
        }

    }

    if(argsRead != 4) {
        printf("Wrong input\n");
        return -1;
    }

    return 0;
}

void removeNewLine(char * buffer) {
    if(buffer[strlen(buffer) - 1] == '\n')
        buffer[strlen(buffer) - 1] = '\0';
}

int memErrCheck(void * ptr) {
    if(ptr == NULL) {
        printf("Error occurred during malloc\n");
        return -1;
    }

    return 0;
}

int initRecordBuffer(patientRecord * buffer) {
    buffer->lastName = malloc(MAX_STRING_LEN);
    if(memErrCheck(buffer->lastName))
        return -1;

    buffer->firstName = malloc(MAX_STRING_LEN);
    if(memErrCheck(buffer->firstName))
        return -1;

    buffer->patientId = malloc(MAX_STRING_LEN);
    if(memErrCheck(buffer->patientId))
        return -1;

    buffer->country = malloc(MAX_STRING_LEN);
    if(memErrCheck(buffer->country))
        return -1;

    buffer->diseaseId = malloc(MAX_STRING_LEN);
    if(memErrCheck(buffer->diseaseId))
        return -1;

    return 0;
}

void freeRecordBuffer(patientRecord * buffer) {
    free(buffer->diseaseId);
    free(buffer->patientId);
    free(buffer->country);
    free(buffer->firstName);
    free(buffer->lastName);
}

int readLine(char * line, patientRecord * buffer) {
    char * entryDate, * exitDate;
    entryDate = malloc(MAX_STRING_LEN);
    if(memErrCheck(entryDate))
        return -1;

    exitDate = malloc(MAX_STRING_LEN);
    if(memErrCheck(exitDate))
        return -1;

    sscanf(line,"%s %s %s %s %s %s %s",buffer->patientId,buffer->firstName,buffer->lastName,\
    buffer->diseaseId,buffer->country,entryDate,exitDate);

    // convert dates
    getDate(&buffer->entryDate,entryDate);
    getDate(&buffer->exitDate,exitDate);

    free(entryDate);
    free(exitDate);
    return 0;
}

void getDate(date * dest, char * strDate) {
    if(strcmp(strDate,"-") == 0) {
        dest->day = 0;
        dest->month = 0;
        dest->year = 0;
    }
    else
        sscanf(strDate,"%d-%d-%d",&dest->day,&dest->month,&dest->year);
}

void copyDate(date * dest, date const * src) {
    dest->day = src->day;
    dest->month = src->month;
    dest->year = src->year;
}

int cmpDates(date const * d1, date const * d2) {
    if(d1->year > d2->year)
        return 1;
    else if(d1->year < d2->year)
        return -1;

    if(d1->month > d2->month)
        return 1;
    else if(d1->month < d2->month)
        return -1;

    if(d1->day > d2->day)
        return 1;
    else if(d1->day < d2->day)
        return -1;

    return 0;
}