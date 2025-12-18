#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "usine_avl.h"       
#include "../Parser/Parser.h" 
#include "Data.h"

#define TYPE_CAPACITE_MAX 1 
#define TYPE_SOURCE_USINE 2 
#define TYPE_USINE_STOCKAGE 3

int max(int a, int b) {
    if(a > b){
        return a;
    }else{
        return b;
    }
}

int hauteur_node(Facility *n) {
    if (n == NULL) {
        return 0; 
    }
    return n->hauteur;
}

int equilibrage(Facility* n){
    if(n==NULL){
        return 0;
    }
    return hauteur_node(n->droite) - hauteur_node(n->gauche);
}

Facility *rotation_gauche_usine(Facility *a) {
    Facility *b = a->droite;
    Facility *T2 = b->gauche; 

    b->gauche = a;
    a->droite = T2; 

    a->hauteur =  1 + max(hauteur_node(a->gauche), hauteur_node(a->droite));
    b->hauteur =  1 + max(hauteur_node(b->gauche), hauteur_node(b->droite));

    return b; 
}

Facility *rotation_droite_usine(Facility *a) {
    Facility *b = a->gauche;
    Facility *T2 = b->droite; 

    b->droite = a;
    a->gauche = T2; 

    a->hauteur = max(hauteur_node(a->gauche), hauteur_node(a->droite)) + 1;
    b->hauteur = max(hauteur_node(b->gauche), hauteur_node(b->droite)) + 1;

    return b; 
}

Facility *rotation_droite_gauche_usine(Facility *n) {
    n->droite = rotation_droite_usine(n->droite);
    return rotation_gauche_usine(n);
}

Facility *rotation_gauche_droite_usine(Facility *n) {
    n->gauche = rotation_gauche_usine(n->gauche);
    return rotation_droite_usine(n);
}

Facility *equilibrer_arbre(Facility *n) {
    if (n == NULL) return n;

    n->hauteur = max(hauteur_node(n->gauche), hauteur_node(n->droite)) + 1;

    int balance = equilibrage(n);

    if (balance > 1) { 
        if (equilibrage(n->droite) >= 0) {
            return rotation_gauche_usine(n);
        } else { 
            return rotation_droite_gauche_usine(n);
        }
    } else if (balance < -1) { 
        if (equilibrage(n->gauche) <= 0) { 
            return rotation_droite_usine(n);
        } else { 
            return rotation_gauche_droite_usine(n);
        }
    }
    return n;
}

Facility *nouvelle_usine(char *id, double capacite_max) {
    Facility *new_node = malloc(sizeof(Facility));
    if (new_node == NULL) {
        return NULL;
    }
    
    strncpy(new_node->id, id, 49); 
    new_node->id[49] = '\0';

    new_node->volume = 0.0;
    new_node->leak = 0.0;
    new_node->parent_id[0] = '\0';
    
    new_node->capacite_max = capacite_max;
    new_node->gauche = NULL;
    new_node->droite = NULL;
    new_node->volume_traite = 0.0;
    new_node->volume_capte = 0.0;
    new_node->hauteur = 1;
    
    return new_node;
}
Facility *inserer_usine(Facility *racine, char *id, double vol_info, double pourcentage_fuite, int type_ligne) {
    if (racine == NULL) {
        if (type_ligne == TYPE_CAPACITE_MAX) {
            return nouvelle_usine(id, vol_info);
        }
        if (type_ligne == TYPE_SOURCE_USINE || type_ligne == TYPE_USINE_STOCKAGE) {
            return nouvelle_usine(id, 0); 
        }
        return NULL; 
    }

    int cmp = strcmp(id, racine->id);

    if (cmp < 0) {
        racine->gauche = inserer_usine(racine->gauche, id, vol_info, pourcentage_fuite, type_ligne);
    } else if (cmp > 0) {
        racine->droite = inserer_usine(racine->droite, id, vol_info, pourcentage_fuite, type_ligne);
    } else {
        if (type_ligne == TYPE_CAPACITE_MAX) {
            racine->capacite_max = vol_info;
        } else if (type_ligne == TYPE_SOURCE_USINE) {
            racine->volume_capte += vol_info; 
            double volume_net_traite = vol_info *(1.0 -(pourcentage_fuite/100.0));
            racine->volume_traite += volume_net_traite;
        } 
        return racine;
    }

    return equilibrer_arbre(racine);
}

void free_avl_usine(Facility* racine){
    if(racine ==NULL){
        return;
    }
    free_avl_usine(racine->gauche);
    free_avl_usine(racine->droite);
    free(racine);
}

 Facility* creerAVLMax(char* nom_fichier){
    FILE* fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        perror("Erreur ouverture fichier");
        return NULL;
    }
    
    char ligne[500];
    Facility* arbre_usines = NULL;

    while (fgets(ligne, sizeof(ligne), fichier)) {

        Facility* f = parserLine(ligne);
        if (f == NULL){
            continue;
        }
       
        if (f->type == FACILITY_COMPLEX) {
            arbre_usines = inserer_usine(arbre_usines,f->id,f->volume, 0.0,TYPE_CAPACITE_MAX);
        }
        free(f);
    }

    fclose(fichier);
    return arbre_usines;
}

Facility *inserer_usine_leak(Facility *racine, char *id, double perte) {
    if (racine == NULL) {
        Facility *new_node = malloc(sizeof(Facility));
        if (new_node == NULL){
            printf("Erreur d'allocation mémoire");
            return NULL;
        }

        // Initialisation complète
        strncpy(new_node->id, id, 49);
        new_node->id[49] = '\0';   
        new_node->parent_id[0] = '\0';
        new_node->type = NONE;      
        new_node->volume = 0.0;
        new_node->capacite_max = 0.0;
        new_node->volume_capte = 0.0;
        new_node->volume_traite = 0.0;
        new_node->volume_perdu = perte;
        new_node->gauche = NULL;
        new_node->droite = NULL;
        new_node->hauteur = 1;

        return new_node;
    }

    int cmp = strcmp(id, racine->id);

    if (cmp < 0) {
        racine->gauche = inserer_usine_leak(racine->gauche, id, perte);
    } else if (cmp > 0) {
        racine->droite = inserer_usine_leak(racine->droite, id, perte);
    } else {
        // Noeud existant : cumul des pertes
        racine->volume_perdu += perte;
    }

    // Rééquilibrage AVL
    return equilibrer_arbre(racine);
}

Facility* creerAVLLeaks(char* nom_fichier){
    FILE* fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        printf("Erreur ouverture fichier %s\n", nom_fichier);
        return NULL;
    }
    
    char ligne[500];
    Facility* arbre_leaks = NULL;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        Facility* f = parserLine(ligne);
        if (f == NULL){
            continue;
        }

        if (f->type == SOURCE || f->type == SPRING) {
            arbre_leaks = inserer_usine_leak(arbre_leaks, f->parent_id, f->volume_perdu);
        }
        free(f);
    }

    fclose(fichier);
    return arbre_leaks;
}
