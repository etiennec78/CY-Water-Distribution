#include <stdio.h>
#include <string.h>
#include "../Data/usine_avl.h"

void histogramme(char* db_path, char* histo_type) {
    printf("DB: %s, histo_type: %s\n", db_path, histo_type);

    Facility* avl;
    if (strcmp(histo_type, "max") == 0) {
        avl = creerAVLMax(db_path);
        printf("Capacité maximale: %f\n", avl->capacite_max);
        free_avl_usine(avl);
    }
}
