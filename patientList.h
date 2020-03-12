//
// Created by theo on 12/3/20.
//

#ifndef SYSPRO_1_PATIENTLIST_H
#define SYSPRO_1_PATIENTLIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "structs.h"
#include "stdbool.h"


void copyDate(date * dest, date const * src);
patientRecord * initRecord(patientRecord buffer);
listNode * initRecordNode(patientRecord buffer);

#endif //SYSPRO_1_PATIENTLIST_H
