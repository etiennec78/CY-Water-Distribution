#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

LineType detectLineType(char* col1, char* col2, char* col3, char* col4, char* col5) {

    if (strcmp(col1, "-") == 0 && strstr(col2, "Spring") != NULL && strstr(col3, "Facility complex") != NULL
        && strcmp(col4, "-") != 0 && strcmp(col5, "-") != 0) {
        return SOURCE_TO_FACTORY;
    }

    if (strcmp(col1, "-") == 0 && strstr(col2, "Facility complex") != NULL
        && strcmp(col3, "-") == 0 && strcmp(col4, "-") != 0 && strcmp(col5, "-") == 0) {
        return FACTORY_ONLY;
    }

    if (strcmp(col1, "-") == 0 && strstr(col2, "Facility complex") != NULL
        && strstr(col3, "Storage") != NULL && strcmp(col4, "-") == 0 && strcmp(col5, "-") != 0) {
        return FACTORY_TO_STORAGE;
    }

    if (strstr(col1, "Facility complex") != NULL && strstr(col2, "Storage") != NULL
        && strstr(col3, "Junction") != NULL && strcmp(col4, "-") == 0 && strcmp(col5, "-") != 0) {
        return STORAGE_TO_JUNCTION;
    }

    if (strstr(col1, "Facility complex") != NULL && strstr(col2, "Junction") != NULL
        && strstr(col3, "Service") != NULL && strcmp(col4, "-") == 0 && strcmp(col5, "-") != 0) {
        return JUNCTION_TO_SERVICE;
    }

    if (strstr(col1, "Facility complex") != NULL && strstr(col2, "Service") != NULL
        && strstr(col3, "Cust") != NULL && strcmp(col4, "-") == 0 && strcmp(col5, "-") != 0) {
        return SERVICE_TO_CUST;
    }

    return UNKNOWN;
}


Facility* parserLine(char* lineStr) {
    char* col1 = strtok(lineStr, ";");
    char* col2 = strtok(NULL, ";");
    char* col3 = strtok(NULL, ";");
    char* col4 = strtok(NULL, ";");
    char* col5 = strtok(NULL, ";");

    Facility* facility = malloc(sizeof(Facility));
    if (facility == NULL){
        return NULL;
    }

    LineType lineType = detectLineType(col1, col2, col3, col4, col5);

    switch(lineType) {
        case SOURCE_TO_FACTORY:
            facility->type = conversionCharToType(col2);  // col2 = Spring
            strcpy(facility->id, col2);
            strcpy(facility->parent_id, col3);           // col3 = Facility complex
            facility->volume = atof(col4);
            facility->leak = atof(col5);
            break;

        case FACTORY_ONLY:
            facility->type = conversionCharToType(col2);  // col2 = Facility complex
            strcpy(facility->id, col2);
            facility->parent_id[0] = '\0';                // Pas de parent
            facility->volume = atof(col4);
            facility->leak = 0;
            break;

        case FACTORY_TO_STORAGE:
            facility->type = conversionCharToType(col3);  // col3 = Storage
            strcpy(facility->id, col3);
            strcpy(facility->parent_id, col2);           // col2 = Facility complex
            facility->volume = atof(col4);
            facility->leak = 0;
            break;

        case STORAGE_TO_JUNCTION:
            facility->type = conversionCharToType(col3);  // col3 = Junction
            strcpy(facility->id, col3);
            strcpy(facility->parent_id, col2);           // col2 = Storage
            facility->volume = atof(col4);
            facility->leak = 0;
            break;

        case JUNCTION_TO_SERVICE:
            facility->type = conversionCharToType(col3);  // col3 = Service
            strcpy(facility->id, col3);
            strcpy(facility->parent_id, col2);           // col2 = Junction
            facility->volume = atof(col4);
            facility->leak = 0;
            break;

        case SERVICE_TO_CUST:
            facility->type = conversionCharToType(col3);  // col3 = Cust
            strcpy(facility->id, col3);
            strcpy(facility->parent_id, col2);           // col2 = Service
            facility->volume = atof(col4);
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
