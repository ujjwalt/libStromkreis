    //
    //  Structures.h
    //  libStromkreis
    //
    //  Created by Ujjwal Thaakar on 04/09/11.
    //  Copyright 2011 Ujjwal Thaakar. All rights reserved.
    //


#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdbool.h>
#include "Global.h"

typedef enum SOPOS {
    SOP,
    POS
} SOPOS;

typedef struct record {
    unsigned *vals, valCount;
    unsigned *diffs, diffCount;
    bool used;
    struct record *next;
} record;

typedef struct group {
    record *records;
    struct group *next;
} group;

typedef struct column {
    group *groups;
} column;

    // Record manipulation

unsigned * copyUnsigned(unsigned *arr, unsigned arrCount);

record * copyRecord(record *rec);

record * insertRecord(record *first, unsigned *vals, unsigned valCount, unsigned *diffs, unsigned diffCount);

void deleteAllRecords(record *first);

record * appendRecords(record *first, record *append);

record * unusedRecords(group *grp);

    // Group manipulation

group * insertGroup(group *first, record *rec);

void deleteAllGroups(group *first);

    // Column manipulation

column * createColumn(group *grp);

void deleteColumn(column *col);

#endif // STRUCTURES_H
