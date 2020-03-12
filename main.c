
#include "miscFunctions.h"
#include "list.h"

int main(int argc, char * argv[]) {
    char * recordsFileName;
    int diseaseHashSize, countryHashSize, bucketSize;
    int retVal = 0;
    FILE * fp;

    listNode * recordsListHead = NULL;

    //read the command line arguments
    retVal = getArguments(argc, argv, &diseaseHashSize, &countryHashSize, &bucketSize, &recordsFileName);
    if(retVal)
        exit(-1);

    //open the file with the records
    retVal = openFile(&fp, recordsFileName);
    if(retVal)
        exit(-1);

    patientRecord recordBuffer;
    retVal = initRecordBuffer(&recordBuffer);
    if(retVal)
        exit(-1);

    char * lineBuffer = NULL;
    size_t lineBufferSize = 0;

    bool recordInserted;

    //read file with records and initialize data structs
    while(getline(&lineBuffer, &lineBufferSize, fp) != -1) {
        removeNewLine(lineBuffer);
        retVal = readLine(lineBuffer,&recordBuffer);
        if(retVal)
            exit(-1);

        recordInserted = checkInsertRecord(&recordsListHead, recordBuffer, &retVal);
        if(retVal)
            exit(-1);
        if(!recordInserted)
            printf("Could not insert patient, he is a duplicate.\n");
    }


    //free allocated space
    freeList('p',&recordsListHead);
    freeRecordBuffer(&recordBuffer);
    free(lineBuffer);
    free(recordsFileName);
    fclose(fp);
    exit(0);
}
