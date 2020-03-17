
#include "miscFunctions.h"

int main(int argc, char * argv[]) {
    char * recordsFileName;
    int diseaseHashSize, countryHashSize, bucketSize;
    int retVal = 0;
    FILE * fp;

    listNode * recordsListHead = NULL;
    avlNode * avlRoot = NULL;
    hashTable * diseaseHashTbl = NULL;
    hashTable * countryHashTbl = NULL;



    //read the command line arguments
    retVal = getArguments(argc, argv, &diseaseHashSize, &countryHashSize, &bucketSize, &recordsFileName);
    if(retVal)
        exit(-1);

    //open the file with the records
    retVal = openFile(&fp, recordsFileName);
    if(retVal)
        exit(-1);

    diseaseHashTbl = initHashTable(diseaseHashSize, bucketSize);
    countryHashTbl = initHashTable(countryHashSize, bucketSize);

    patientRecord recordBuffer; // a buffer to store record read from each line
    retVal = initRecordBuffer(&recordBuffer);
    if(retVal)
        exit(-1);

    char * lineBuffer = NULL;
    size_t lineBufferSize = 0;

    listNode * recordPointer = NULL;

    //read file with records and initialize data structs
    while(getline(&lineBuffer, &lineBufferSize, fp) != -1) {
        removeNewLine(lineBuffer);
        retVal = readLine(lineBuffer,&recordBuffer);
        if(retVal)
            exit(-1);

        retVal = insertToStructs(&recordsListHead,&recordBuffer,&recordPointer,diseaseHashTbl,countryHashTbl);
        if(retVal)
            exit(-1);
    }

    char * inputBuffer;
    inputBuffer = initStrBuffer(MAX_LINE_LEN);
    if(inputBuffer == NULL)
        exit(-1);

    char * command;
    command = initStrBuffer(ARG_LEN);
    if(command == NULL)
        exit(-1);

    printf("\n\tPlease enter a command\n>> ");

    retVal = getInput(inputBuffer,MAX_LINE_LEN); // read user input from stdin
    if(retVal)
        exit(-1);

    getCommand(inputBuffer,command,ARG_LEN);

    //start the user interface
    while(strcmp(command,"exit") != 0){
        if(strcmp(command,"/insertPatientRecord") == 0) {
            if(getArgumentsNum(inputBuffer) != 8 && getArgumentsNum(inputBuffer) != 7) {
                printf("Check your input\n");
            }
            else { // get patient data and insert to data structures
                retVal = readInput(inputBuffer, &recordBuffer);
                if(retVal)
                    exit(-1);

                retVal = insertToStructs(&recordsListHead,&recordBuffer,&recordPointer,diseaseHashTbl,countryHashTbl);
                if(retVal)
                    exit(-1);
            }
        }
        else if(strcmp(command,"/printDiseaseHash") == 0) {
            printHashTable(diseaseHashTbl);
        }
        else if(strcmp(command,"/printCountryHash") == 0) {
            printHashTable(countryHashTbl);
        }
        else if(strcmp(command,"/globalDiseaseStats") == 0) {
            date start;
            date end;
            if(getArgumentsNum(inputBuffer) == 1)
                getAllDiseaseStats(diseaseHashTbl,false,start,end);
            else if(getArgumentsNum(inputBuffer) == 3) {
                getDates(inputBuffer,2,3,&start,&end);
                if(!datesCorrect(&start,&end))
                    printf("The dates specified are incorrect\n");
                else
                    getAllDiseaseStats(diseaseHashTbl,true,start,end);
            }
            else
                printf("Wrong input\n");
        }
        else if(strcmp(command,"/diseaseFrequency") == 0) {
            date start;
            date end;
            char virusName[ARG_LEN];
            getNthArgument(inputBuffer,virusName,2);

            if(getArgumentsNum(inputBuffer) == 4) {
                getDates(inputBuffer,3,4,&start,&end);
                if(!datesCorrect(&start,&end))
                    printf("The dates specified are incorrect\n");
                else
                    getDiseaseStats(diseaseHashTbl,virusName,false,NULL,start,end);
            }
            else if(getArgumentsNum(inputBuffer) == 5) {
                char country[ARG_LEN];
                getNthArgument(inputBuffer,country,3);

                getDates(inputBuffer,4,5,&start,&end);
                if(!datesCorrect(&start,&end))
                    printf("The dates specified are incorrect\n");
                else
                    getDiseaseStats(diseaseHashTbl,virusName,true,country,start,end);
            }
            else
                printf("Wrong input\n");
        }
        else if(strcmp(command,"/topk-Diseases") == 0) {
            retVal = processInputAndCallHeap(inputBuffer,'d',countryHashTbl);
            if(retVal)
                exit(-1);
        }
        else if(strcmp(command,"/topk-Countries") == 0) {
            retVal = processInputAndCallHeap(inputBuffer,'c',diseaseHashTbl);
            if(retVal)
                exit(-1);
        }
        else if(strcmp(command,"/recordPatientExit") == 0) {
            if(getArgumentsNum(inputBuffer) == 3) {
                char patientId[ARG_LEN];
                getNthArgument(inputBuffer,patientId,2);
                date newDate;
                char dateStr[ARG_LEN];
                getNthArgument(inputBuffer,dateStr,3);
                getDate(&newDate,dateStr);

                updateExitDate(recordsListHead,newDate,patientId);
            }
            else
                printf("Wrong Input\n");
        }
        else if(strcmp(command,"/numCurrentPatients") == 0) {
            if(getArgumentsNum(inputBuffer) == 2) {
                char disease[ARG_LEN];
                getNthArgument(inputBuffer,disease,2);
                getDiseaseSickPatients(diseaseHashTbl,disease);
            }
            else
                getAllSickPatients(diseaseHashTbl);
        }
        else {
            printf("No such command\n");
        }

        printf("\n\n>> ");
        retVal = getInput(inputBuffer,MAX_LINE_LEN); // read user input from stdin
        if(retVal)
            exit(-1);

        getCommand(inputBuffer,command,ARG_LEN);
    }

    //free allocated space
    freeHashTable(&diseaseHashTbl);
    freeHashTable(&countryHashTbl);
    freeAvlTree(&avlRoot);
    freeRecordsList(&recordsListHead);
    freeRecordBuffer(&recordBuffer);
    free(lineBuffer);
    free(recordsFileName);
    free(inputBuffer);
    free(command);
    fclose(fp);
    exit(0);
}

