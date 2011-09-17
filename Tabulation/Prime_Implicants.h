    //
    //  Prime_Implicants.h
    //  libStromkreis
    //
    //  Created by Ujjwal Thaakar on 04/09/11.
    //  Copyright 2011 Ujjwal Thaakar. All rights reserved.
    //

#ifndef PRIME_IMPLICANTS_H
#define PRIME_IMPLICANTS_H

#include "Tabulation.h"

    // Form Records

record * formRecords(void);

    // Form Groups

group * formGroups(record *);

    // Valid Record pair

bool validRecordPair(record *, record *);

    // Combine records

record * combineRecords(record *, record *);

    // Compare groups

record * compareGroups(group *, group *);

    // Delete records which have not been eliminated and form them into a linked list

record * unusedRecords(group *);

    // Process Column

bool processColumn(column *);

    // Get prime implicants

void removeRedundantRecords(record *);

void removeRedundantDiffs(record *);

record * primeImplicants(void);

#endif // PRIME_IMPLICANTS_H
