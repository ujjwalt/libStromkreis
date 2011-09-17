    //
    //  Essential_Prime_Implicants.c
    //  libStromkreis
    //
    //  Created by Ujjwal Thaakar on 10/09/11.
    //  Copyright 2011 Ujjwal Thaakar. All rights reserved.
    //

#include "Essential_Prime_Implicants.h"
#include "Prime_Implicants.h"
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

bool **table;
unsigned rows, columns;

extern unsigned *values, valueCount;

record *primeImps = NULL;

void setTable(record *primeImps) {
    unsigned primeImpsCount = 0; // No. of prime implicants
    record *temp = primeImps;
        // Traverse the prime implicants to count the total no.
    while (temp != NULL) {
        primeImpsCount++;
        temp = temp->next;
    }
    columns = primeImpsCount; // Set the no. of columns
    rows = valueCount; // Set the no. of rows
    
        // Allocate memory for table
    if (UNEXPECTED((table = (bool **)calloc(rows, sizeof(bool *))) == NULL)) {
        fprintf(stderr, "Not enough memory");
        return;
    }
    for (unsigned i = 0; i < rows; i++) {
        if (UNEXPECTED((table[i] = (bool *)calloc(columns, sizeof(bool))) == NULL)) {
            fprintf(stderr, "Not enough memory");
            return;
        }
    }
    
        // Set columns
    temp = primeImps;
    while (temp != NULL) {
        for (unsigned i = 0; i < temp->valCount; i++) {
            unsigned valIndex = indexForVal(temp->vals[i]);
            unsigned impIndex = indexForImp(temp);
            table[valIndex][impIndex] = true;
        }
        temp = temp->next;
    }
}

unsigned indexForVal(unsigned val) {
        // Return index for the specified value from the input boolean function
    for (unsigned i = 0; i < valueCount; i++)
        if (values[i] == val)
            return i;
    return UINT_MAX;
}

unsigned indexForImp(record *rec) {
        // Return Index for the implicant rec
    record *temp = primeImps;
    for (unsigned i = 0; i < columns; i++, temp = temp->next) {
        if (rec == temp)
            return i;
    }
    return UINT_MAX;
}

record * valForIndex(unsigned index) {
        // Return the implicant for the given index
    record *temp = primeImps;
    for (unsigned i = 0; i < index; i++, temp = temp->next);
    return temp;
}


record * minimalPrimeImplicants(void) {
    primeImps = primeImplicants();
    setTable(primeImps);
    
        // Set essential prime implicants
    record *new = NULL; // Holds the minimal terms
    
    
        // Remove and add essential prime implicants
    unsigned lastI;
    unsigned rowsToDel[rows], rowsToDelCount = 0;
    unsigned colsToDel[columns], colsToDelCount = 0;
    for (unsigned i = 0; i < rows; i++) {
        unsigned truthValue = 0;
        for (unsigned j = 0; j < columns; j++) {
            if (table[i][j]) {
                truthValue += table[i][j];
                lastI = j;
            }
        }
            // Check if the term is only covered by one min term
        if (truthValue == 1) {
                // Add the appropriate min term
            new = appendRecords(new, copyRecord(valForIndex(lastI)));
            
                // Estimate which rows and colums have to be deleted
            
                // Remove rows which are true at lastI
            for (unsigned k = 0; k < rows; k++) {
                if (table[k][lastI]) {
                        // Make sure the row has not already been added
                    bool rowAdded = false;
                    for (unsigned s = 0; s < rowsToDelCount; s++) {
                        if (rowsToDel[s] == k)
                            rowAdded = true;
                    }
                    if (!rowAdded)
                        rowsToDel[rowsToDelCount++] = k;   
                }
            }
            
                // Remove the lastI column if not already added
            bool colAdded = false;
            for (unsigned s = 0; s < colsToDelCount; s++) {
                if (colsToDel[s] == lastI)
                    colAdded = true;
            }
            if (!colAdded)
                colsToDel[colsToDelCount++] = lastI;
        }
    }
    
    
        // Determine what subtable of the table to create based on the rows and columns to del
        // 1 extra memory block is padded so as to avoid a crash in case rowsToDelCount == rows
        // and colsToDelCount == cols
    unsigned rowsToKeep[rows-rowsToDelCount+1], rowsToKeepCount = 0, colsToKeep[columns-colsToDelCount+1], colsToKeepCount = 0;
    for (unsigned i = 0; i < rows; i++) {
        bool rowToDelete = false;
        for (unsigned j = 0; j < rowsToDelCount; j++) {
            if (i == rowsToDel[j])
                rowToDelete = true;
        }
        if (!rowToDelete)
            rowsToKeep[rowsToKeepCount++] = i;
    }
    
    for (unsigned i = 0; i < columns; i++) {
        bool colToDelete = false;
        for (unsigned j = 0; j < colsToDelCount; j++) {
            if (i == colsToDel[j])
                colToDelete = true;
        }
        if (!colToDelete)
            colsToKeep[colsToKeepCount++] = i;
    }
    bool **newTable;
    
    if (UNEXPECTED((newTable = (bool **)calloc(rowsToKeepCount, sizeof(bool *))) == NULL)) {
        fprintf(stderr, "Not enough memory");
        return NULL;
    }
    for (unsigned i = 0; i < rowsToKeepCount; i++) {
        if (UNEXPECTED((newTable[i] = (bool *)calloc(colsToKeepCount, sizeof(bool))) == NULL)) {
            fprintf(stderr, "Not enough memory");
            return NULL;
        }
    }
    
    for (unsigned i = 0; i < rowsToKeepCount; i++) {
        for (unsigned j = 0; j < colsToKeepCount; j++) {
            memcpy(&newTable[i][j], &table[rowsToKeep[i]][colsToKeep[j]], sizeof(bool));
        }
    }
    
        // Free old table
    for (unsigned i = 0; i < rows; i++) {
        free(table[i]);
    }
    
    columns = colsToKeepCount;
    rows = rowsToKeepCount;
    table = newTable;
    
    if (!rows || !columns)
        rows = columns = 0;
    
    
    unsigned options = ~(~0U << columns);
    for (unsigned i = 0; i < rows; i++) {
            // Process each column;
        unsigned iOptions = 0U;
        for (unsigned j = 0; j < columns; j++) {
            if (table[i][j])
                iOptions = iOptions | (1U << j);
        }
        options = options & iOptions;
    }
    
        // Determine the terms and add them to new
    for (unsigned a = 1U, i = 0; a <= options; a <<= 1, i++) {
        if (options & a) {
            record *temp = valForIndex(i);
            new = appendRecords(new, copyRecord(temp));
        }
    }
    
    removeRedundantRecords(new);
    removeRedundantDiffs(new);
    
    return new;
}
