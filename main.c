
#include "miscFunctions.h"
#include "patientList.h"

int main(int argc, char * argv[]) {
    char * recordsFileName;
    int diseaseHashSize, countryHashSize, bucketSize;
    int retVal;
    FILE * fp;

    //read the command line arguments
    retVal = getArguments(argc, argv, &diseaseHashSize, &countryHashSize, &bucketSize, &recordsFileName);
    if(retVal)
        exit(-1);

    //open the file with the records
    retVal = openFile(&fp, recordsFileName);
    if(retVal)
        exit(-1);

    //read file with records
    char * lineBuffer = NULL;
    size_t lineBufferSize = 0;
    while(getline(&lineBuffer, &lineBufferSize, fp) != -1) {
        removeNewLine(lineBuffer);
        printf("buffer content: %s (newln check)\n", lineBuffer);
    }


    //free allocated space
    free(lineBuffer);
    free(recordsFileName);
    fclose(fp);
    exit(0);
}
