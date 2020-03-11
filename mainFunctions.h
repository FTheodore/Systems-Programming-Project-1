//
// Created by theo on 11/3/20.
//

#ifndef SYSPRO_1_MAINFUNCTIONS_H
#define SYSPRO_1_MAINFUNCTIONS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int openFile(FILE ** fp, char * fileName);
int getArguments(int argc, char ** argv, int * diseaseHashSize, int * countryHashSize, int * bucketSize, char ** fileName);

#endif //SYSPRO_1_MAINFUNCTIONS_H
