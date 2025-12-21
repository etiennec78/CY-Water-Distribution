#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "../Data/common.h"
#include "../Data/avl_histo.h"


LineType detect_line_type(char** cols) {
    // Security check: ensure all necessary columns are present
    for (int i = 0; i < COL_LEN; i++) {
        if (cols[i] == NULL) {
            return UNKNOWN;
        }
    }

    int c1_dash = (strcmp(cols[0], "-") == 0);
    int c3_dash = (strcmp(cols[2], "-") == 0);
    int c4_dash = (strcmp(cols[3], "-") == 0);
    int c5_dash = (strcmp(cols[4], "-") == 0);
    
    // Determine line type based on which columns contain "-"
    if (c1_dash && c3_dash && c5_dash) {    
        return FACTORY_ONLY;
    } else if (c1_dash && c4_dash) {
        return FACTORY_TO_STORAGE;
    } else if (c1_dash) {
        return SOURCE_TO_FACTORY;
    } else if (c4_dash) {
        return STORAGE_TO_CUST; //contains the last 3 line types
    }
    
    return UNKNOWN;
}

Facility* parse_line(char* lineStr, Facility* factory_tree) {
    char* cols[COL_LEN];
    // Explicit initialization to NULL to avoid invalid memory access
    for (int i = 0; i < COL_LEN; i++) {
        cols[i] = NULL;
    }

    // Split the line
    char* token = strtok(lineStr, ";");
    int i = 0;
    while (token != NULL && i < COL_LEN) {
        cols[i] = token;
        token = strtok(NULL, ";");
        i++;
    }

    int h = 0;
    LineType lineType = detect_line_type(cols);

    // Insert into AVL tree based on line type
    switch(lineType) {
        case SOURCE_TO_FACTORY:
            // ID is in col 2, Volume in col 3, Leak in col 4
            factory_tree = insert_facility(factory_tree, cols[2], atof(cols[3]), atof(cols[4]), SOURCE_TO_FACTORY, &h);
            break;

        case FACTORY_ONLY:
            // ID is in col 1, Capacity in col 3
            factory_tree = insert_facility(factory_tree, cols[1], atof(cols[3]), 0, FACTORY_ONLY, &h);
            break;

        case FACTORY_TO_STORAGE:
            // ID is in col 1, Volume in col 3
            factory_tree = insert_facility(factory_tree, cols[1], atof(cols[3]), 0, FACTORY_TO_STORAGE, &h);
            break;

        default:
            break;
    }

    return factory_tree;
}
