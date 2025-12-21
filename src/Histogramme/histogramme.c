#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "histogramme.h"
#include "../Data/avl_histo.h"


double get_capacite_max(Facility* f) {
    return f->capacite_max;
}

double get_volume_capte(Facility* f) {
    return f->volume_capte;
}

double get_volume_traite(Facility* f) {
    return f->volume_traite;
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

void print_avl_reverse(Facility* node, FILE* f, GetterList* getters) {
    if (node == NULL) return;
    print_avl_reverse(node->droite, f, getters);

    fprintf(f, "%s", node->id);
    for (int i=0; i<getters->length; i++) {
        fprintf(f, ";%f", getters->list[i](node));
    }
    fprintf(f, "\n");
    print_avl_reverse(node->gauche, f, getters);
}

void histogramme(char* db_path, char* histo_type) {
    Facility* avl;
    char* filePath;
    char* dataTitle;
    GetterList* getters = NULL;

    avl = creerAVLMax(db_path);
    if (avl == NULL) {
        fprintf(stderr, "Erreur: Impossible de créer l'arbre AVL (fichier vide ou mal formé)\n");
        return;
    }

    if (strcmp(histo_type, "max") == 0) {
        filePath = "data/vol_max.dat";
        dataTitle = "max volume(M.m3/year)";
        getters = append_getter(getters, get_capacite_max);
    }
    else if (strcmp(histo_type, "src") == 0) {
        filePath = "data/vol_captation.dat";
        dataTitle = "source volume(M.m3/year)";
        getters = append_getter(getters, get_volume_capte);
    }
    else if (strcmp(histo_type, "real") == 0) {
        filePath = "data/vol_traitement.dat";
        dataTitle = "real volume(M.m3/year)";
        getters = append_getter(getters, get_volume_traite);
    }
    else if (strcmp(histo_type, "all") == 0) {
        filePath = "data/histo_all.dat";
        dataTitle = "identifier;max volume(M.m3/year);source volume(M.m3/year);real volume(M.m3/year)";
        getters = append_getter(getters, get_capacite_max);
        getters = append_getter(getters, get_volume_capte);
        getters = append_getter(getters, get_volume_traite);
    }
    else{
        printf("Argument non valide");
        return;
    }

    FILE* f = fopen(filePath, "w");
    if (f == NULL) {
        perror("Erreur création fichier de sortie");
        free_avl_usine(avl);
        return;
    }

    char title[256] = "";
    strcat(title, "identifier;");
    strcat(title, dataTitle);
    strcat(title, "\n");
    fprintf(f, "%s", title);
    print_avl_reverse(avl, f, getters);

    fclose(f);

    free_avl_usine(avl);
    free_getters(getters);
}
