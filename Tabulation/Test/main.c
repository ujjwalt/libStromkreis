#include <stdio.h>
#include <stdlib.h>
#include "Tabulation.h"

int main()
{
    unsigned litCount, valCount;
    printf("Enter number of literals/variables :-\n");
    scanf("%u", &litCount);
    char lits[litCount];
    printf("Enter the literals/variables :-\n");
    scanf("%s", lits);
    
    printf("Enter number of values :-\n");
    scanf("%u", &valCount);
    if (!valCount) {
        printf("Invalid number of values !\n");
        return -1;
    }
    
    unsigned vals[valCount];
    printf("Enter the values :-\n");
    for (int i = 0; i < valCount; i++) {
        scanf("%u", vals+i);
    }
    setLiteralsAndValues(lits, litCount, vals, valCount);
    
    printf("Enter wether to calculate the SOP/POS - enter 1/0: ");
    unsigned opt;
    scanf("%u", &opt);
    
    if (opt)
        setSOPOSType(SOP);
    else
        setSOPOSType(POS);
    
    char *c = simplifiedTermsAsString();
    printf("\n\nSolution :-\n%s", c);
    return 0;
}
