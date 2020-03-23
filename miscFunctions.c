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

void printRecord(listNode * patientRec) {
    patientRecord * dataPtr = patientRec->dataPointer;

    printf("Patient id: %s\n\tName: %s %s\n\tDisease id: %s\n\tCountry: %s\n",\
        dataPtr->patientId,dataPtr->firstName,dataPtr->lastName,dataPtr->diseaseId,dataPtr->country);
    printf("\tEntry date: %d-%d-%d\n",dataPtr->entryDate.day,dataPtr->entryDate.month,dataPtr->entryDate.year);
    printf("\tExit date: %d-%d-%d\n",dataPtr->exitDate.day,dataPtr->exitDate.month,dataPtr->exitDate.year);
    printf("\t~~~~~~~~~~~~~~~~\n\n");
}

int getInput(char * buff, int buffSize) {
    // stores the data from stdin stream(tty) inside the buff
    char c;
    int i;
    for (i = 0; i < buffSize && (c = getchar()) != EOF && c != '\n'; ++i)
    {
        if(i != 0 && c == ' ')
        {
            buff[i] = ' ';
            i += 1;
            if(i >= buffSize)
                break;
        }

        while(c == ' ') // skip whitespaces
            c = getchar();

        if(c == '\n' || c == EOF)
            break;

        buff[i] = c;
    }

    if(i >= buffSize)
    {
        printf("Error while processing user input, please check buffer size\n");
        return -1;
    }

    i -= 1;

    while(i >= 0 && buff[i] == ' ')
        i -= 1;

    buff[i + 1] = '\0';

    return 0;
}

void getCommand(char const * buff, char * comm, int lim) {
    // comm will contain the first word that is stored in buff
    int i;
    for (i = 0; i < lim - 1 && buff[i] != ' ' && buff[i] != '\0'; ++i)
        comm[i] = buff[i];
    comm[i] = '\0';
}

int getArgumentsNum(char const * buff) {
    // returns the number of words stored in buff
    int count = 0;
    for (int i = 0; i <= strlen(buff); ++i)
    {
        if((buff[i] == ' ' || buff[i] == '\0') && i != 0)
            count += 1;
    }
    return count;
}

void getNthArgument(char const * buff, char * argmnt, int n) {
    // argmnt will contain the n-th word stored in buff
    if(n > getArgumentsNum(buff)) {
        argmnt[0] = '\0';
        return;
    }

    int count = 0;
    int i;
    int y = 0;
    for (i = 0; i <= strlen(buff); ++i)
    {
        if(buff[i] == ' ' || buff[i] == '\0')
        {
            count += 1;
            if(i < strlen(buff))
                i += 1;
        }

        if(count == n)
            break;

        if(count == n - 1)
        {
            argmnt[y] = buff[i];
            y += 1;
        }
    }
    argmnt[y] = '\0';
}

char * initStrBuffer(int bufferSize) {
    char * newBuff;

    newBuff = malloc(bufferSize);
    if(memErrCheck(newBuff))
        return NULL;

    return newBuff;
}

void getAndCopy(char * inputBuff,int n,char * dest) {
    char arg[ARG_LEN];
    getNthArgument(inputBuff,arg,n);
    strcpy(dest,arg);
}

int readInput(char * inputBuff, patientRecord * buffer) { // read data about a patient from stdin
    char * entryDate, * exitDate;
    entryDate = malloc(MAX_STRING_LEN);
    if(memErrCheck(entryDate))
        return -1;

    exitDate = malloc(MAX_STRING_LEN);
    if(memErrCheck(exitDate))
        return -1;

    getAndCopy(inputBuff,2,buffer->patientId);
    getAndCopy(inputBuff,3,buffer->firstName);
    getAndCopy(inputBuff,4,buffer->lastName);
    getAndCopy(inputBuff,5,buffer->diseaseId);
    getAndCopy(inputBuff,6,buffer->country);
    getAndCopy(inputBuff,7,entryDate);

    if(getArgumentsNum(inputBuff) == 8)
        getAndCopy(inputBuff,8,exitDate);
    else
        strcpy(exitDate,"-");


    // convert dates
    getDate(&buffer->entryDate,entryDate);
    getDate(&buffer->exitDate,exitDate);

    free(entryDate);
    free(exitDate);
    return 0;
}

int insertToStructs(listNode ** recordsListHead, patientRecord * recordBuffer, listNode ** recordPointer,\
hashTable * diseaseHashTbl, hashTable * countryHashTbl) {
    bool recordInserted;
    int retVal = 0;

    //check if dates are correct
    if(!datesCorrect(&recordBuffer->entryDate,&recordBuffer->exitDate)) {
        printf("Could not insert patient with id #%s\n",recordBuffer->patientId);
        return 0;
    }

    // insert record to list
    recordInserted = checkInsertRecord(recordsListHead, *recordBuffer, &retVal, recordPointer);
    if(retVal)
        return -1;

    if(!recordInserted) {
        printf("Could not insert patient with id #%s, he is a duplicate\n",recordBuffer->patientId);
        return 0;
    }

    // insert to hash tables
    patientRecord * patientData = (*recordPointer)->dataPointer;
    retVal = insertToHashTable(diseaseHashTbl, patientData->diseaseId, &patientData->entryDate, *recordPointer);
    if(retVal)
        return -1;

    retVal = insertToHashTable(countryHashTbl, patientData->country, &patientData->entryDate, *recordPointer);
    if(retVal)
        return -1;
}

bool datesCorrect(date * entryDate, date * exitDate) {
    if(exitDate->year == 0 && exitDate->month == 0 && exitDate->day == 0)
        return true;

    if(cmpDates(entryDate,exitDate) <= 0)
        return true;

    printf("The dates given are incorrect\n");
    return false;
}

void getDates(char * inputBuff,int startDateInd,int endDateInd,date * start,date * end) {
    char arg[ARG_LEN];

    getNthArgument(inputBuff,arg,startDateInd);
    getDate(start,arg);

    getNthArgument(inputBuff,arg,endDateInd);
    getDate(end,arg);
}

int processInputAndCallHeap(char * inputBuffer,char type,hashTable * hash) {
    date start;
    date end;
    char string[ARG_LEN];
    getNthArgument(inputBuffer,string,3);
    char num[ARG_LEN];
    getNthArgument(inputBuffer,num,2);
    int retVal;

    if(getArgumentsNum(inputBuffer) == 3) {
        retVal = getEntryTopValues(hash,string,atoi(num),type,false,start,end);
        if(retVal)
            return -1;
    }
    else if(getArgumentsNum(inputBuffer) == 5) {
        getDates(inputBuffer,4,5,&start,&end);
        if(!datesCorrect(&start,&end))
            return 0;

        retVal = getEntryTopValues(hash,string,atoi(num),type,true,start,end);
        if(retVal)
            return -1;
    }
    else
        printf("Wrong input\n");

    return 0;
}

bool noValue(date dt) {
    if(dt.day == 0 && dt.month == 0 && dt.year == 0)
        return true;

    return false;
}