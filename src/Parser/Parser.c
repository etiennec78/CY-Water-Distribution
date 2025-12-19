#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parser.h"
#include "../Data/Data.h"
#include "../Data/usine_avl.h"

LineType detectLineType(char** cols) {
    if (cols == NULL || cols[0] == NULL || cols[1] == NULL || cols[2] == NULL) {
        return UNKNOWN;
    }

    int c1_dash = (strcmp(cols[0], "-") == 0);
    int c3_dash = (strcmp(cols[2], "-") == 0);
    int c4_dash = (strcmp(cols[3], "-") == 0);
    int c5_dash = (strcmp(cols[4], "-") == 0);
    

    if (c1_dash && c3_dash && c5_dash) {    
        return FACTORY_ONLY;
    } else if(c1_dash && c4_dash){
        return FACTORY_TO_STORAGE;
    } else if(c1_dash){
        return SOURCE_TO_FACTORY;
    } else if(c4_dash){
        return STORAGE_TO_CUST; //contient les 3 derniers types de ligne
    }
    
    return UNKNOWN;
}

Facility* parserLine(char* lineStr, Facility* arbre_usines) {
    char* cols[COL_LEN];
    for (int i=0; i<COL_LEN; i++) {
        cols[i] = strtok(lineStr, ";");
        lineStr = NULL;
    }

    int h = 0;
    LineType lineType = detectLineType(cols);

    switch(lineType) {
        case SOURCE_TO_FACTORY:
            arbre_usines = inserer_usine(arbre_usines, cols[2], atof(cols[3]), atof(cols[4]), SOURCE_TO_FACTORY, &h);
            break;

        case FACTORY_ONLY:
            arbre_usines = inserer_usine(arbre_usines, cols[1], atof(cols[3]), 0, FACTORY_ONLY, &h);
            break;

        case FACTORY_TO_STORAGE:
            arbre_usines = inserer_usine(arbre_usines, cols[1], atof(cols[3]), 0, FACTORY_TO_STORAGE, &h);
            break;

        // case STORAGE_TO_CUST :
        //     arbre_usines = inserer_usine(arbre_usines, cols[2], atof(cols[3]), 0, STORAGE_TO_CUST, &h);  // contient les 3 derniers types de ligne
        //     break;

        default:
            break;
    }

    return arbre_usines;
}
