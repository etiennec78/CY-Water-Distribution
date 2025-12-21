#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "histogram.h"
#include "../Data/avl_histo.h"


double get_max_capacity(Facility* f) {
    return f->max_capacity;
}

double get_captured_volume(Facility* f) {
    return f->captured_volume;
}

double get_treated_volume(Facility* f) {
    return f->treated_volume;
}

GetterList* create_getters(capacity_getter newGetter) {
    GetterList* getters = malloc(sizeof(GetterList));
    getters->list = malloc(sizeof(capacity_getter));
    getters->list[0] = newGetter;
    getters->length = 1;
    return getters;
}

GetterList* append_getter(GetterList* getters, capacity_getter newGetter) {
    if (getters == NULL) {
        return create_getters(newGetter);
    }
    getters->list = realloc(getters->list, sizeof(capacity_getter)*(getters->length+1));
    getters->list[getters->length] = newGetter;
    getters->length++;
    return getters;
}

void free_getters(GetterList* getters) {
    free(getters->list);
    free(getters);
}

// Go through the AVL in reverse order to print the facilities in descending order based on their IDs
void print_avl_reverse(Facility* node, FILE* f, GetterList* getters) {
    if (node == NULL) return;
    print_avl_reverse(node->right, f, getters);

    fprintf(f, "%s", node->id);
    for (int i=0; i<getters->length; i++) {
        fprintf(f, ";%f", getters->list[i](node));
    }
    fprintf(f, "\n");
    print_avl_reverse(node->left, f, getters);
}

// Generate a .dat file containing data for drawing an histogram
void histogram(char* db_path, char* histo_type) {
    Facility* avl;
    char* filePath;
    char* dataTitle;
    GetterList* getters = NULL;

    avl = create_avl_max(db_path);
    if (avl == NULL) {
        fprintf(stderr, "Erreur: Impossible de créer l'arbre AVL (fichier vide ou mal formé)\n");
        return;
    }

    // Set the file to store the data, the title of the csv headers, and append getters to retrieve the data in a loop
    if (strcmp(histo_type, "max") == 0) {
        filePath = "data/vol_max.dat";
        dataTitle = "max volume(M.m3/year)";
        getters = append_getter(getters, get_max_capacity);
    }
    else if (strcmp(histo_type, "src") == 0) {
        filePath = "data/vol_captation.dat";
        dataTitle = "source volume(M.m3/year)";
        getters = append_getter(getters, get_captured_volume);
    }
    else if (strcmp(histo_type, "real") == 0) {
        filePath = "data/vol_traitement.dat";
        dataTitle = "real volume(M.m3/year)";
        getters = append_getter(getters, get_treated_volume);
    }
    else if (strcmp(histo_type, "all") == 0) {
        filePath = "data/histo_all.dat";
        dataTitle = "max volume(M.m3/year);source volume(M.m3/year);real volume(M.m3/year)";
        getters = append_getter(getters, get_max_capacity);
        getters = append_getter(getters, get_captured_volume);
        getters = append_getter(getters, get_treated_volume);
    }
    else {
        printf("Argument non valide");
        return;
    }

    FILE* f = fopen(filePath, "w");
    if (f == NULL) {
        perror("Erreur création fichier de sortie");
        free_avl_facility(avl);
        return;
    }

    // Build and save the header
    char title[256] = "";
    strcat(title, "identifier;");
    strcat(title, dataTitle);
    strcat(title, "\n");
    fprintf(f, "%s", title);
    print_avl_reverse(avl, f, getters);

    // Free all pointers
    fclose(f);
    free_avl_facility(avl);
    free_getters(getters);
}
