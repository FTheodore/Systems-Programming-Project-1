//
// Created by theo on 11/3/20.
//

#include "miscFunctions.h"

int openFile(FILE ** fp, char * fileName) {
    char *absolutePath;
    absolutePath = malloc(strlen(fileName) + strlen("../") + 1);
    if(absolutePath == NULL) {
        printf("Error occurred during malloc\n");
        return -1;
    }
    strcpy(absolutePath,"../");
    strcat(absolutePath,fileName);

    if((*fp = fopen(absolutePath,"r")) == NULL) {
        printf("Error, couldn't open the records file\n");
        return -1;
    }
    free(absolutePath);
    return 0;
}

int getArguments(int argc, char ** argv, int * diseaseHashSize, int * countryHashSize, int * bucketSize, char ** fileName) {
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
            if(*fileName == NULL) {
                printf("Error occurred during malloc\n");
                return -1;
            }
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