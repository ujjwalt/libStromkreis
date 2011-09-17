    //
    //  Essential_Prime_Implicants.h
    //  libStromkreis
    //
    //  Created by Ujjwal Thaakar on 10/09/11.
    //  Copyright 2011 Ujjwal Thaakar. All rights reserved.
    //

#ifndef ESSENTIAL_PRIME_IMPLICANTS_H
#define ESSENTIAL_PRIME_IMPLICANTS_H

#include "Structures.h"

void setTable(record *);

unsigned indexForVal(unsigned);

unsigned indexForImp(record *);

record * valForIndex(unsigned index);

record * minimalPrimeImplicants(void);

#endif // ESSENTIAL_PRIME_IMPLICANTS_H
