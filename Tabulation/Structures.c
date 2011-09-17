    //
    //  Structures.c
    //  libStromkreis
    //
    //  Created by Ujjwal Thaakar on 04/09/11.
    //  Copyright 2011 Ujjwal Thaakar. All rights reserved.
    //

#include "Structures.h"
#include <stdlib.h>
#include <string.h>

    // Record manipulation

unsigned * copyUnsigned(unsigned *arr, unsigned arrCount) {
    if (arr == NULL)
        return NULL;
    
    unsigned *new = NULL;
    if (UNEXPECTED((new = (unsigned *)malloc(arrCount * sizeof(unsigned))) == NULL)) {
        fprintf(stderr, "Not enough memory");
    } else {
        memcpy(new, arr, sizeof(unsigned)*arrCount);
    }
    return new;
}

record * copyRecord(record *rec) {
    record *new = NULL;
    if (UNEXPECTED((new = (record *)malloc(sizeof(record))) == NULL)) {
        fprintf(stderr, "Not enough memory");
    } else {
        new->valCount = rec->valCount;
        new->vals = copyUnsigned(rec->vals, rec->valCount);
        new->diffCount = rec->diffCount;
        new->diffs = copyUnsigned(rec->diffs, rec->diffCount);
        new->used = false;
        new->next = NULL;
    }
    return new;
}

record * insertRecord(record *first, unsigned *vals, unsigned valCount, unsigned *diffs, unsigned diffCount) {
    record *new = NULL;
    if (UNEXPECTED((new = (record *)malloc(sizeof(record))) == NULL)) {
        fprintf(stderr, "Not enough memory");
        return NULL;
    } else {
        new->valCount = valCount;
        new->vals = vals;
        new->diffCount = diffCount;
        new->diffs = diffs;
        new->used = false;
        new->next = NULL;
    }
    
    if (UNEXPECTED(first == NULL)) {
        first = new;
    } else {
        record *temp = first;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new;
    }
    return first;
}

void deleteAllRecords(record *first) {
    record *temp;
    while (first != NULL) {
        temp = first;
        first = first->next;
        free(temp->vals);
        free(temp->diffs);
        free(temp);
    }
}

record * appendRecords(record *first, record *append) {
    if (first == NULL)
        return append;
    record *temp = first;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = append;
    return first;
}

    // Group manipulation

group * insertGroup(group *first, record *rec) {
    group *new = NULL;
    if (UNEXPECTED((new = (group *)malloc(sizeof(group))) == NULL)) {
        fprintf(stderr, "Not enough memory");
        return NULL;
    } else {
        new->records = rec;
        new->next = NULL;
    }
    
    if (UNEXPECTED(first == NULL)) {
        first = new;
    } else {
        group *temp = first;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new;
    }
    return first;
}

void deleteAllGroups(group *first) {
    group *temp;
    while (first != NULL) {
        temp = first;
        first = first->next;
        deleteAllRecords(temp->records);
        free(temp);
    }
}

    // Column manipulation

column * createColumn(group *grp) {
    column *new = NULL;
    if (UNEXPECTED((new = (column *)malloc(sizeof(column))) == NULL)) {
        fprintf(stderr, "Not enough memory");
    } else {
        new->groups = grp;
    }
    return new;
}

void deleteColumn(column *col) {
    deleteAllGroups(col->groups);
    free(col);
}
