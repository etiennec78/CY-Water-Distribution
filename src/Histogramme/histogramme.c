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

void print_avl_reverse_max(Facility* node, FILE* f) {
    if (node == NULL) return;
    print_avl_reverse_max(node->droite, f);
    if(node->type_ligne == FACTORY_ONLY) fprintf(f, "%s;%f\n", node->id, node->capacite_max);
    print_avl_reverse_max(node->gauche, f);
}

void print_avl_reverse_src(Facility* node, FILE* f) {
    if (node == NULL) return;
    print_avl_reverse_src(node->droite, f);
    fprintf(f, "%s;%f\n", node->id, node->volume_capte);
    print_avl_reverse_src(node->gauche, f);
}

void print_avl_reverse_real(Facility* node, FILE* f) {
    if (node == NULL) return;
    print_avl_reverse_real(node->droite, f);
    fprintf(f, "%s;%f\n", node->id, node->volume_traite);
    print_avl_reverse_real(node->gauche, f);
}

void histogramme(char* db_path, char* histo_type) {
    Facility* avl;
    char* filePath;
    char* dataTitle;

    avl = creerAVLMax(db_path);
    if (avl == NULL) {
        fprintf(stderr, "Erreur: Impossible de créer l'arbre AVL (fichier vide ou mal formé)\n");
        return;
    }


    if (strcmp(histo_type, "max") == 0) {
        filePath = "data/vol_max.dat";

        FILE* f = fopen(filePath, "w");
        if (f == NULL) {
            perror("Erreur création fichier de sortie");
            free_avl_usine(avl);
            return;
        }

        fprintf(f, "usine_id;maximum_volume\n");
        print_avl_reverse_max(avl, f);

        fclose(f);

    }
    else if (strcmp(histo_type, "src") == 0) {
        filePath = "data/vol_captation.dat";

        FILE* f = fopen(filePath, "w");
        if (f == NULL) {
            perror("Erreur création fichier de sortie");
            free_avl_usine(avl);
            return;
        }

        fprintf(f, "usine_id;source_volume\n");
        print_avl_reverse_src(avl, f);
    
        fclose(f);
    }
    else if (strcmp(histo_type, "real") == 0){
        filePath = "data/vol_traitement.dat";

        FILE* f = fopen(filePath, "w");
        if (f == NULL) {
            perror("Erreur création fichier de sortie");
            free_avl_usine(avl);
            return;
        }

        fprintf(f, "usine_id;treated_volume\n");
        print_avl_reverse_real(avl, f);
    
        fclose(f);

    }
    else{
        printf("Argument non valide");
        return;
    }

    free_avl_usine(avl);
}
