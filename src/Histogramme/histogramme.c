#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "histogramme.h"
#include "../Data/usine_avl.h"


double get_capacite_max(Facility* f) {
    return f->capacite_max;
}

double get_volume_capte(Facility* f) {
    return f->volume_capte;
}

double get_volume_traite(Facility* f) {
    return f->volume_traite;
}

void print_avl_reverse(Facility* node, FILE* f, capacity_getter getter) {
    if (node == NULL) return;
    print_avl_reverse(node->droite, f, getter);
    fprintf(f, "%s;%f\n", node->id, getter(node));
    print_avl_reverse(node->gauche, f, getter);
}

void histogramme(char* db_path, char* histo_type) {
    Facility* avl;
    char* filePath;
    capacity_getter getter;
    char* dataTitle;

    avl = creerAVLMax(db_path);
    if (avl == NULL) {
        fprintf(stderr, "Erreur: Impossible de créer l'arbre AVL (fichier vide ou mal formé)\n");
        return;
    }

    if (strcmp(histo_type, "max") == 0) {
        filePath = "data/vol_max.dat";
        getter = get_capacite_max;
        dataTitle = "Capacité maximale";
    }
    else if (strcmp(histo_type, "src") == 0) {
        filePath = "data/vol_captation.dat";
        getter = get_volume_capte;
        dataTitle = "Volume capté";
    }
    else if (strcmp(histo_type, "real") == 0){
        filePath = "data/vol_traitement.dat";
        getter = get_volume_traite;
        dataTitle = "Volume traité";
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

    char title[50];
    strcat(title, "Usine");
    strcat(title, dataTitle);
    strcat(title, "\n");
    fprintf(f, "%s", title);
    print_avl_reverse(avl, f, getter);

    fclose(f);
    free_avl_usine(avl);
}
