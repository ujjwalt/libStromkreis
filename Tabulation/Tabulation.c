    //
    //  Tabulation.c
    //  libStromkreis
    //
    //  Created by Ujjwal Thaakar on 04/09/11.
    //  Copyright 2011 Ujjwal Thaakar. All rights reserved.
    //

#include "Tabulation.h"
#include "Essential_Prime_Implicants.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static SOPOS calcType;
static bool processRunning = false, literalsSet = false;

unsigned *values, valueCount = 0U, literalCount = 0U;
char *literals;

    // Set literals and values

void setLiteralsAndValues(char *lits, unsigned litCount, unsigned *vals, unsigned valCount) {
        // Check wether process is already running
    if (processRunning) {
        fprintf(stderr, "Process already started - literals and values cannot be set at this stage");
        return;
    }
    
        // Copy the literals and values
    if (UNEXPECTED((literals = (char *)malloc(litCount * sizeof(char))) == NULL)) {
        fprintf(stderr, "Not enough memory");
        return;
    } else {
        memcpy(literals, lits, sizeof(unsigned)*litCount);
    }
    
    values = copyUnsigned(vals, valCount);
    literalCount = litCount;
    valueCount = valCount;
    literalsSet = true;
}

    // Set SOP/POS Type

void setSOPOSType(SOPOS type) {
    if (processRunning) {
        fprintf(stderr, "Process already started - literals and values cannot be set at this stage");
        return;
    }
    if (!literalsSet) {
        fprintf(stderr, "Literals and values not set yet !");
        return;
    }
    calcType = type;
}

    // Get string for simplified terms

char * simplifiedTermsAsString(void) {
        // Get simplified terms
    record *first = minimalPrimeImplicants();
    
        // Get the number of minial prime implicants
    unsigned count = 0;
    record *temp = first;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    
        // Create a array of strings to hold each individual term
    if (count == 0)
        count++;
    char *master[count];
    unsigned masterCount = 0;
    
        // Process each term and create a string for it
    while (first != NULL) {
            // Get the value to mask (the greatest value)
        unsigned val = first->vals[first->valCount-1];
        unsigned *maskPos = first->diffs;
        unsigned terms = literalCount - first->diffCount;
        if (calcType == POS)
            terms = 2*terms+1;
        unsigned mask = 0U;
        for (unsigned i = 0; i < first->diffCount; i++) {
            mask = mask | maskPos[i];
        }
        
        for (unsigned a = 1U; a <= pow(2, literalCount-1); a <<= 1) {
            unsigned bit = mask & a;
            if (!bit) {
                if (calcType == SOP) {
                    if (!(val & a))
                        terms++;
                } else {
                    if (val&a) {
                        terms++;
                    }
                }
            }
        }
        
            // Process according to wether its a POS/SOP
        char *str;
        if (UNEXPECTED((str = (char *)malloc((terms+1)*sizeof(char))) == NULL)) {
            fprintf(stderr, "Not enough memory");
            return NULL;
        }
        str[terms] = '\0';
        
        
        
        if (calcType == SOP) {
            for (int i = literalCount-1,  j = terms-1, a  = 1U; j >= 0; i--, a<<=1) {
                unsigned bit = mask&a;
                if (!bit) {
                    if (!(val & a)) {
                        str[j--] = '`';
                    }
                    str[j--] = literals[i];
                }
            }
        } else {
            str[0] = '('; str[--terms] = ')';
            for (int i = literalCount-1,  j = terms-1, a  = 1U; j >= 1; i--, a<<=1) {
                unsigned bit = mask&a;
                if (!bit) {
                    if ((val & a)) {
                        str[j--] = '`';
                    }
                    str[j--] = literals[i];
                    j--;
                    if (j+1 > 1)
                        str[j+1] = '+';
                }
            }
        }
        
        master[masterCount++] = str;
        first = first->next;
    }
    
    int len = 0;
    for (unsigned i = 0; i < masterCount; i++) {
        len += strlen(master[i]);
    }
    char *simple;
    
    if (UNEXPECTED((simple = (char *)calloc(len+1, sizeof(char))) == NULL)) {
        fprintf(stderr, "Not enough memory");
        return NULL;
    }
    
    simple[len] = '\0';
    for (unsigned i = 0; i < masterCount; i++) {
        strcat(simple, master[i]);
        if (calcType == SOP && i != masterCount-1)
            strcat(simple, "+");
    }
    
    return simple;
}
