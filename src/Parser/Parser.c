#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Data/Data.h"




// int countLine(){
//     FILE* fichier = NULL;
//     fichier = fopen("c-wildwater_v3.dat", "r");

//     if(fichier == NULL){
//         printf("Can not open file");
//         exit(1);
//     }

//     int c;
//     int nbLignes = 0;
//     while((c = fgetc(fichier)) != EOF){  
//         if(c == '\n'){
//             nbLignes++;
//         }
//     }
//     printf("Il y a %d lignes dans le fichier", nbLignes);
//     return nbLignes;
//}

FacilityType conversionCharToType(char* mot){
    char* type[19] = {"SOURCE", "SPRING", "POND", "CATCHMENT", "WELL", "RESURGENCE", "INTAKE", "BOREHOLE", "FOUNTAIN", "RESERVOIR", "FORAGE", "WELL_FIELDS", "TANK", "UNIT", "MODULE", "FACILITY_COMPLEX", "STORAGE", "JUNCTION", "CUST"};
    FacilityType facilityType[19] = {SOURCE, SPRING, POND, CATCHMENT, WELL, RESURGENCE, INTAKE, BOREHOLE, FOUNTAIN, RESERVOIR, FORAGE, WELL_FIELDS, TANK, UNIT, MODULE, FACILITY_COMPLEX, STORAGE, JUNCTION, CUST};
    
    for(int i = 0; i < 19; i++){
        if(strcmp(mot, type[i]) == 0){
            return facilityType[i];
        }
    }
    return -1;
}

Capture* parserTabCapture(){
    FILE* fichier = NULL;
    fichier = fopen("c-wildwater_v3.dat", "r");

    if(fichier == NULL){
        printf("Can not open file");
        exit(1);
    }

    Capture* tableau = malloc(sizeof(Capture));
    if (tableau == NULL) {
        printf("Memory allocation failed");
        exit(1);
    }
    int j = 1;
    int res;
    char* upstream;
    char* facility;
    while(res=(fscanf(fichier, "-;%s %s;%s %s;%d;%f\n", upstream, tableau[j].upstreamID, facility, tableau[j].facilityID, &tableau[j].annualAmount, &tableau[j].leakPercentage))==6){
        tableau[j].upstream = conversionCharToType(upstream);
        tableau[j].facility = conversionCharToType(facility);
        tableau = realloc(tableau, sizeof(Capture) * (j + 1));
        if (tableau == NULL) {
            printf("Memory reallocation failed");
            exit(1);
        }
        j++;
    }

    fclose(fichier);
    return tableau;
}



Factory* parserTabFactory(){
    FILE* fichier = NULL;
    fichier = fopen("c-wildwater_v3.dat", "r");

    if(fichier == NULL){
        printf("Can not open file");
        exit(1);
    }

    Factory* tableau = malloc(sizeof(Factory));
    int j = 0;
    int res;
    char* facility;
    while(res = fscanf(fichier, "-;%s %s;-;%d;-\n", facility, tableau[j].facilityID, &tableau[j].annualAmount)==3){
        tableau[j].facility = conversionCharToType(facility);
        j++;
        tableau = realloc(tableau, sizeof(Factory) * (j + 1));
        if (tableau == NULL) {
            printf("Memory reallocation failed");
            exit(1);
        }
        j++;
    }

    fclose(fichier);
    return tableau;
}


Storage* parserTabStorage(){
    FILE* fichier = NULL;
    fichier = fopen("c-wildwater_v3.dat", "r");

    if(fichier == NULL){
        printf("Can not open file");
        exit(1);
    }

    Storage* tableau = malloc(sizeof(Storage));
    int j = 0;
    int res;
    char* facility;
    char* downstream;
    while(res = fscanf(fichier, "-;%s %s;%s %s;-;%f\n", facility, tableau[j].facilityID, downstream, tableau[j].downstreamID, &tableau[j].leakPercentage)==5){
        tableau[j].facility = conversionCharToType(facility);
        tableau[j].downstream = conversionCharToType(downstream);
        j++;
        tableau = realloc(tableau, sizeof(Storage) * (j + 1));
        if (tableau == NULL) {
            printf("Memory reallocation failed");
            exit(1);
        }
        j++;
    }

    fclose(fichier);
    return tableau;
}

principalDistribution* parserTabPrincipalDistribution(){
    FILE* fichier = NULL;
    fichier = fopen("c-wildwater_v3.dat", "r");

    if(fichier == NULL){
        printf("Can not open file");
        exit(1);
    }

    principalDistribution* tableau = malloc(sizeof(principalDistribution));
    int j = 0;
    int res;
    char* facility;
    char* downstream;
    char* junction;
    while(res = fscanf(fichier, "%s %s;%s %s;%s %s;-;%f\n", facility, tableau[j].facilityID, downstream, tableau[j].downstreamID, junction, tableau[j].junctionID, &tableau[j].leakPercentage)==7){
        tableau[j].facility = conversionCharToType(facility);
        tableau[j].downstream = conversionCharToType(downstream);
        tableau[j].junction = conversionCharToType(junction);
        j++;
        tableau = realloc(tableau, sizeof(principalDistribution) * (j + 1));
        if (tableau == NULL) {
            printf("Memory reallocation failed");
            exit(1);
        }
        j++;
    }

    fclose(fichier);
    return tableau;
}

secondaryDistribution* parserTabSecondaryDistribution(){
    FILE* fichier = NULL;
    fichier = fopen("c-wildwater_v3.dat", "r");

    if(fichier == NULL){
        printf("Can not open file");
        exit(1);
    }

    secondaryDistribution* tableau = malloc(sizeof(secondaryDistribution));
    int j = 0;
    int res;
    char* facility;
    char* downstream;
    char* junction;
    while(res = fscanf(fichier, "%s %s;%s %s;%s %s;-;%f\n", facility, tableau[j].facilityID, downstream, tableau[j].downstreamID, junction, tableau[j].junctionID, &tableau[j].leakPercentage)==7){
        tableau[j].facility = conversionCharToType(facility);
        tableau[j].downstream = conversionCharToType(downstream);
        tableau[j].junction = conversionCharToType(junction);
        j++;
        tableau = realloc(tableau, sizeof(secondaryDistribution) * (j + 1));
        if (tableau == NULL) {
            printf("Memory reallocation failed");
            exit(1);
        }
        j++;
    }

    fclose(fichier);
    return tableau;
}

connexion* parserTabConnexion(){
    FILE* fichier = NULL;
    fichier = fopen("c-wildwater_v3.dat", "r");

    if(fichier == NULL){
        printf("Can not open file");
        exit(1);
    }

    connexion* tableau = malloc(sizeof(connexion));
    int j = 0;
    int res;
    char* facility;
    char* downstream;
    char* junction;
    while(res = fscanf(fichier, "%s %s;%s %s;%s %s;-;%f\n", facility, tableau[j].facilityID, downstream, tableau[j].downstreamID, junction, tableau[j].junctionID, &tableau[j].leakPercentage)==7){
        tableau[j].facility = conversionCharToType(facility);
        tableau[j].downstream = conversionCharToType(downstream);
        tableau[j].junction = conversionCharToType(junction);
        j++;
        tableau = realloc(tableau, sizeof(connexion) * (j + 1));
        if (tableau == NULL) {
            printf("Memory reallocation failed");
            exit(1);
        }
        j++;
    }

    fclose(fichier);
    return tableau;
}

