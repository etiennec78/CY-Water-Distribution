#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Data/usine_avl.h"

void print_avl_reverse(Facility* node, FILE* f) {
    if (node == NULL) return;
    print_avl_reverse(node->droite, f);
    fprintf(f, "%s;%f\n", node->id, node->capacite_max);
    print_avl_reverse(node->gauche, f);
}

void histogramme(char* db_path, char* histo_type) {

    Facility* avl;
    if (strcmp(histo_type, "max") == 0) {
        avl = creerAVLMax(db_path);
        if (avl == NULL) {
            fprintf(stderr, "Erreur: Impossible de créer l'arbre AVL (fichier vide ou mal formé)\n");
            return;
        }
        
        FILE* f = fopen("data/vol_max.dat", "w");
        if (f == NULL) {
            perror("Erreur création fichier vol_max.dat");
            free_avl_usine(avl);
            return;
        }
        
        fprintf(f, "Usine;Capacite\n");
        print_avl_reverse(avl, f);
        
        fclose(f);
        free_avl_usine(avl);
    }
}
