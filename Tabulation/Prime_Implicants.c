    //
    //  Prime_Implicants.c
    //  libStromkreis
    //
    //  Created by Ujjwal Thaakar on 04/09/11.
    //  Copyright 2011 Ujjwal Thaakar. All rights reserved.
    //

#include "Prime_Implicants.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static record *primeImps = NULL;
extern unsigned *values, valueCount;
extern char *literals, literalCount;

    // Form Records

record * formRecords(void) {
    record *new = NULL;
    for (unsigned i = 0; i < valueCount; i++) {
        unsigned *val = copyUnsigned(values+i, 1);
        new = insertRecord(new, val, 1, NULL, 0);
    }
    return new;
}

    // Form Groups

group * formGroups(record *first) {
    record **groups;
    
    if (UNEXPECTED((groups = (record **)malloc((literalCount+1) * sizeof(record*))) == NULL)) {
        fprintf(stderr, "Not enough memory");
        return NULL;
    }
    
    for (int i = 0; i < literalCount+1; i++)
        groups[i] = NULL;
    
    while (first != NULL) {
        unsigned onesCount = 0;
        for (unsigned a = 1U; a <= *first->vals; a <<= 1) {
            if (a & *first->vals) {
                onesCount++;
            }
        }
        unsigned *newVals = copyUnsigned(first->vals, first->valCount);
        unsigned *newDiffs = copyUnsigned(first->diffs, first->diffCount);
        groups[onesCount] = insertRecord(groups[onesCount], newVals, first->valCount, newDiffs, first->diffCount);
        first = first->next;
    }
    
    group *g = NULL;
    for (int i = 0; i < literalCount+1; i++) {
        if (groups[i] != NULL) {
            g = insertGroup(g, groups[i]);
        }
    }    
    return g;
}

    // Valid Record pair

bool validRecordPair(record *r1, record *r2) {
    if (r1->diffCount != r2->diffCount || r1->valCount != r2->valCount)
        return false;
    
    for (unsigned i = 0; i < r1->diffCount; i++)
        if (*(r1->diffs+i) != *(r2->diffs+i))
            return false;
    for (unsigned i = 0; i < r1->valCount; i++) {
        signed difference = *(r2->vals+i) - *(r1->vals+i);
        /*
         Check if log2(difference) is an integer
         */
        double intPart;
        if (difference <= 0 || modf(log2(difference), &intPart) != 0.0)
            if (difference != 1)
                return false;
    }
    return true;
}

    // Combine records

record * combineRecords(record *r1, record *r2) {
    record *newr1 = copyRecord(r1);
    record *newr2 = copyRecord(r2);
    unsigned *buff;
    if (UNEXPECTED((buff = (unsigned *)realloc(newr1->vals, sizeof(unsigned)*(newr1->valCount+newr2->valCount))) == NULL)) {
        fprintf(stderr, "Not enough memory");
        return NULL;
    } else {
        newr1->vals = buff;
        memcpy(newr1->vals+newr1->valCount, newr2->vals, sizeof(unsigned)*newr2->valCount);
        newr1->valCount += newr2->valCount;
    }
    
    if (UNEXPECTED((buff = (unsigned *)realloc(newr1->diffs, sizeof(unsigned)*(newr1->diffCount+newr2->diffCount+1))) == NULL)) {
        fprintf(stderr, "Not enough memory");
        return NULL;
    } else {
        newr1->diffs = buff;
        memcpy(newr1->diffs+newr1->diffCount, newr2->diffs, sizeof(unsigned)*newr2->diffCount);
        newr1->diffCount += newr2->diffCount;
    }
    *(newr1->diffs+newr1->diffCount) = (*(newr2->vals) - *(newr1->vals));
    newr1->diffCount++;
    deleteAllRecords(newr2);
    r1->used = r2->used = true;
    return newr1;
}

    // Compare groups

record * compareGroups(group *g1, group *g2) {
    record *new = NULL;
    record *r1 = g1->records;
    record *r2 = g2->records;
    while (r1 != NULL) {
        record *temp = r2;
        while (temp != NULL) {
            if (validRecordPair(r1, temp)) {
                new = appendRecords(new, combineRecords(r1, temp));
            }
            temp = temp->next;
        }
        r1 = r1->next;
    }
    
    return new;
}

    // Delete records which have not been eliminated and form them into a linked list

record * unusedRecords(group *grp) {
    record *r = grp->records, *new = NULL;
    while (r != NULL) {
        if (!(r->used)) {
            new = appendRecords(new, copyRecord(r));
        }
        r = r->next;
    }
    return new;
}

    // Process Column

bool processColumn(column *col) {
    group *g = col->groups;
    record *new = NULL;
    while(g->next != NULL) {
        new = appendRecords(new, compareGroups(g, g->next));
        primeImps = appendRecords(primeImps, unusedRecords(g));
        g = g->next;
    }
    primeImps = appendRecords(primeImps, unusedRecords(g));
    if (UNEXPECTED(new == NULL)) {
        return true;
    } else {
        column *col;
        if (UNEXPECTED((col = (column *)malloc(sizeof(column))) == NULL)) {
            fprintf(stderr, "Not enough memory");
            return false;
        }
            //removeRedundantRecords(new);
        col->groups = formGroups(new);
        return processColumn(col);
    }
}

    // Get prime implicants

void removeRedundantRecords(record *first) {
        // Remove redundant terms from first
    if (first == NULL)
        return;
    
    while (first->next != NULL) {
        record *pred = first;
        record *comparedTo = first;
        record *temp = first->next;
        while (temp != NULL) {
            bool valsMatch = true;
            for (int i = 0; i < comparedTo->valCount; i++) {
                bool valMatched = false;
                for (int j = 0; j < temp->valCount; j++) {
                    if (comparedTo->vals[i] == temp->vals[j]) {
                        valMatched = true;
                        break;
                    }
                }
                valsMatch = valsMatch && valMatched;
            }
            if (valsMatch) {
                pred->next = temp->next;
                free(temp->vals);
                free(temp->diffs);
                free(temp);
                temp = pred->next;
            } else {
                pred = temp;
                temp = temp->next;
            }
        }
        if ((first = first->next) == NULL)
            return;
    }
}

void removeRedundantDiffs(record *first) {
    if (first == NULL)
        return;
    
    while (first != NULL) {
        unsigned *newdiffs = NULL, newDiffsCount = 0;
        for (int i = 0; i < first->diffCount; i++) {
            bool matched = false;
            for (int j = 0; j < newDiffsCount; j++) {
                if (first->diffs[i] == newdiffs[j]) {
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                newDiffsCount++;
                unsigned *buff;
                if (UNEXPECTED((buff = (unsigned *)realloc(newdiffs, sizeof(unsigned)*newDiffsCount)) == NULL)) {
                    fprintf(stderr, "Not enough memory");
                    return;
                } else {
                    if (newdiffs != buff)
                        free(newdiffs);
                    newdiffs = buff;
                    newdiffs[newDiffsCount-1] = first->diffs[i];
                }
            }
        }
        free(first->diffs);
        first->diffs = newdiffs;
        first->diffCount = newDiffsCount;
        first = first->next;
    }
}

record * primeImplicants(void) {
    record *r = formRecords();
    group *g = formGroups(r);
    column *c;
    if (UNEXPECTED((c = (column *)malloc(sizeof(column))) == NULL)) {
        fprintf(stderr, "Not enough memory");
        return NULL;
    }
    c->groups = g;
    if (EXPECTED(processColumn(c))) {
        removeRedundantRecords(primeImps);
        removeRedundantDiffs(primeImps);
        return primeImps;
    } else {
        fprintf(stderr, "Some error in processing occured\nThe program is absolutely sorry - please try again !");
        return NULL;
    }
}
