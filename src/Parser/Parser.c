#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parser.h"
#include "../Data/Data.h"

FacilityType conversionCharToType(char* mot){
    char* type[19] = {"SOURCE", "SPRING", "POND", "CATCHMENT", "WELL", "RESURGENCE", "INTAKE", "BOREHOLE", "FOUNTAIN", "RESERVOIR", "FORAGE", "WELL_FIELDS", "TANK", "UNIT", "MODULE", "FACILITY_COMPLEX", "STORAGE", "JUNCTION", "CUST"};
    FacilityType facilityType[19] = {SOURCE, SPRING, POND, CATCHMENT, WELL, RESURGENCE, INTAKE, BOREHOLE, FOUNTAIN, RESERVOIR, FORAGE, WELL_FIELDS, TANK, UNIT, MODULE, FACILITY_COMPLEX, STORAGE, JUNCTION, CUST};
    
    for(int i = 0; i < 19; i++){
        if(strcmp(mot, type[i]) == 0){
            return facilityType[i];
        }
    }
    return TYPE_UNKNOWN;
}

LineType detectLineType(char** cols) {

    const char* FACTORY_STRUCTURES[FACTORY_STRUCT_LEN][COL_LEN] = {
        {"-", "Spring", "Facility complex", "-", "-"},
        {"-", "Facility complex", "-", "-", "-"},
        {"-", "Facility complex", "Storage", "-", "-"},
        {"Facility complex", "Storage", "Junction", "-", "-"},
        {"Facility complex", "Junction", "Service", "-", "-"},
        {"Facility complex", "Service", "Cust", "-", "-"}
    };

    const LineType FACTORY_RETURNS[FACTORY_STRUCT_LEN] = {
        SOURCE_TO_FACTORY,
        FACTORY_ONLY,
        FACTORY_TO_STORAGE,
        STORAGE_TO_JUNCTION,
        JUNCTION_TO_SERVICE,
    };

    // For each factory structure
    for (int i=0; i<FACTORY_STRUCT_LEN; i++) {
        int valid = 1;

        // For each factory structure element
        for (int j=0; j<COL_LEN; j++) {

            // Skip this model if the column is different from the column model
            if (strstr(FACTORY_STRUCTURES[i][j], cols[i]) == NULL) {
                valid = 0;
                break;
            }
        }

        // If the loop finished without a break
        if (valid) {
            // Return the related line type
            return FACTORY_RETURNS[i];
        }
    }

    return UNKNOWN;
}


Facility* parserLine(char* lineStr) {
    char* lineIdx = lineStr;
    char* cols[COL_LEN];
    for (int i=0; i<COL_LEN; i++) {
        cols[i] = strtok(lineStr, ";");
        if (lineIdx != NULL) {
            lineIdx = NULL;
        }
    }

    Facility* facility = malloc(sizeof(Facility));
    if (facility == NULL){
        return NULL;
    }

    LineType lineType = detectLineType(cols);

    switch(lineType) {
        case SOURCE_TO_FACTORY:
            facility->type = conversionCharToType(cols[1]);  // col1 = Spring
            strcpy(facility->id, cols[1]);
            strcpy(facility->parent_id, cols[2]);           // col2 = Facility complex
            facility->volume = atof(cols[3]);
            facility->leak = atof(cols[4]);
            break;

        case FACTORY_ONLY:
            facility->type = conversionCharToType(cols[1]);  // col1 = Facility complex
            strcpy(facility->id, cols[1]);
            facility->parent_id[0] = '\0';                // Pas de parent
            facility->volume = atof(cols[3]);
            facility->leak = 0;

            
            break;

        case FACTORY_TO_STORAGE:
            facility->type = conversionCharToType(cols[2]);  // col2 = Storage
            strcpy(facility->id, cols[2]);
            strcpy(facility->parent_id, cols[1]);           // col1 = Facility complex
            facility->volume = atof(cols[3]);
            facility->leak = 0;
            break;

        case STORAGE_TO_JUNCTION:
            facility->type = conversionCharToType(cols[2]);  // col2 = Junction
            strcpy(facility->id, cols[2]);
            strcpy(facility->parent_id, cols[1]);           // col1 = Storage
            facility->volume = atof(cols[3]);
            facility->leak = 0;
            break;

        case JUNCTION_TO_SERVICE:
            facility->type = conversionCharToType(cols[2]);  // col2 = Service
            strcpy(facility->id, cols[2]);
            strcpy(facility->parent_id, cols[1]);           // col1 = Junction
            facility->volume = atof(cols[3]);
            facility->leak = 0;
            break;

        case SERVICE_TO_CUST:
            facility->type = conversionCharToType(cols[2]);  // col2 = Cust
            strcpy(facility->id, cols[2]);
            strcpy(facility->parent_id, cols[1]);           // col1 = Service
            facility->volume = atof(cols[3]);
            facility->leak = 0;
            break;

        default:
            facility->type = TYPE_UNKNOWN;
            facility->id[0] = '\0';
            facility->parent_id[0] = '\0';
            facility->volume = 0;
            facility->leak = 0;
            break;
    }

    return facility;
}
