    //
    //  Tabulation.h
    //  libStromkreis
    //
    //  Created by Ujjwal Thaakar on 04/09/11.
    //  Copyright 2011 Ujjwal Thaakar. All rights reserved.
    //

#ifndef TABULATION_H
#define TABULATION_H

#include "Structures.h"

    // Set literals and values

void setLiteralsAndValues(char *lits, unsigned litCount, unsigned *vals, unsigned valCount);

    // Set SOP/POS Type

void setSOPOSType(SOPOS type);

    // Get string for simplified terms

char * simplifiedTermsAsString(void);

#endif // TABULATION_H
