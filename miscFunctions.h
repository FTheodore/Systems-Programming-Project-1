//
// Created by theo on 11/3/20.
//

#ifndef SYSPRO_1_MISCFUNCTIONS_H
#define SYSPRO_1_MISCFUNCTIONS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "structs.h"


int openFile(FILE ** fp, char * fileName);
int getArguments(int argc, char ** argv, int * diseaseHashSize, int * countryHashSize, int * bucketSize, char ** fileName);
void removeNewLine(char * buffer);

#endif //SYSPRO_1_MISCFUNCTIONS_H
