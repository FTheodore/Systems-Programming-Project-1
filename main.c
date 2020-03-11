
#include "mainFunctions.h"

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


    free(recordsFileName);
    fclose(fp);
    exit(0);
}
