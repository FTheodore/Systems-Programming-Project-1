
#include "patientList.h"
#include "avlTree.h"

int main(int argc, char * argv[]) {
    char * recordsFileName;
    int diseaseHashSize, countryHashSize, bucketSize;
    int retVal = 0;
    FILE * fp;

    listNode * recordsListHead = NULL;
    avlNode * avlRoot = NULL;

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
    listNode * recordPointer = NULL;
    //read file with records and initialize data structs
    while(getline(&lineBuffer, &lineBufferSize, fp) != -1) {
        removeNewLine(lineBuffer);
        retVal = readLine(lineBuffer,&recordBuffer);
        if(retVal)
            exit(-1);

        // insert record to list
        recordInserted = checkInsertRecord(&recordsListHead, recordBuffer, &retVal, &recordPointer);
        if(retVal)
            exit(-1);

        if(!recordInserted) {
            printf("Could not insert patient, he is a duplicate.\n");
            continue;
        }

        // insert record pointer to tree
        retVal = insertToAvlTree(&avlRoot, &recordBuffer.entryDate, recordPointer, avlRoot);
        if(retVal)
            exit(-1);
    }

    printf("\tPRINTING LIST ::\n");
    printRecordsList(recordsListHead);

    //free allocated space
    freeAvlTree(&avlRoot);
    freeRecordsList(&recordsListHead);
    freeRecordBuffer(&recordBuffer);
    free(lineBuffer);
    free(recordsFileName);
    fclose(fp);
    exit(0);
}
