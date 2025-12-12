#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "structure.h"
#include "usine_avl.h"       

#define TYPE_CAPACITE_MAX 1 
#define TYPE_SOURCE_USINE 2 
#define TYPE_USINE_STOCKAGE 3

int max(int a, int b) {
    return (a > b) ? a : b;
}

int get_hauteur_node(UsineNode *n) {
    if (n == NULL) {
        return 0; 
    }
    return n->hauteur;
}

int hauteur_usine(UsineNode* n){
    if(n==NULL){
        return 0;
    }
    return 1 + max(hauteur_usine(n->gauche),hauteur_usine(n->droite));
}

int equilibrage(UsineNode* n){
    if(n==NULL){
        return 0;
    }
    return hauteur_usine(n->droite) - hauteur_usine(n->gauche);
}

UsineNode *rotation_gauche_usine(UsineNode *a) {
    UsineNode *b = a->droite;
    UsineNode *T2 = b->gauche; 

    b->gauche = a;
    a->droite = T2; 

    a->hauteur = max(get_hauteur_node(a->gauche), get_hauteur_node(a->droite)) + 1;
    b->hauteur = max(get_hauteur_node(b->gauche), get_hauteur_node(b->droite)) + 1;

    return b; 
}

UsineNode *rotation_droite_usine(UsineNode *a) {
    UsineNode *b = a->gauche;
    UsineNode *T2 = b->droite; 

    b->droite = a;
    a->gauche = T2; 

    a->hauteur = max(get_hauteur_node(a->gauche), get_hauteur_node(a->droite)) + 1;
    b->hauteur = max(get_hauteur_node(b->gauche), get_hauteur_node(b->droite)) + 1;

    return b; 
}

UsineNode *rotation_droite_gauche_usine(UsineNode *n) {
    n->droite = rotation_droite_usine(n->droite);
    return rotation_gauche_usine(n);
}

UsineNode *rotation_gauche_droite_usine(UsineNode *n) {
    n->gauche = rotation_gauche_usine(n->gauche);
    return rotation_droite_usine(n);
}

UsineNode *equilibrer_arbre(UsineNode *n) {
    if (n == NULL) return n;

    n->hauteur = max(get_hauteur_node(n->gauche), get_hauteur_node(n->droite)) + 1;

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

UsineNode *nouvelle_usine(char *id, long long capacite_max) {
    UsineNode *new_node = malloc(sizeof(UsineNode));
    if (new_node == NULL) {
        return NULL;
    }
    
    new_node->id_usines = strdup(id); 
    if (new_node->id_usines == NULL) {
        free(new_node);
        return NULL;
    }
    
    new_node->capacite_max = capacite_max;
    new_node->gauche = NULL;
    new_node->droite = NULL;
    new_node->volume_traite = 0;
    new_node->volume_capte = 0;
    new_node->hauteur = 1;
    
    return new_node;
}
UsineNode *inserer_usine(UsineNode *racine, char *id, long long vol_info, float pourcentage_fuite, int type_ligne) {
    if (racine == NULL) {
        if (type_ligne == TYPE_CAPACITE_MAX) {
            return nouvelle_usine(id, vol_info);
        }
        if (type_ligne == TYPE_SOURCE_USINE || type_ligne == TYPE_USINE_STOCKAGE) {
            return nouvelle_usine(id, 0); 
        }
        return NULL; 
    }

    int cmp = strcmp(id, racine->id_usines);

    if (cmp < 0) {
        racine->gauche = inserer_usine(racine->gauche, id, vol_info, pourcentage_fuite, type_ligne);
    } else if (cmp > 0) {
        racine->droite = inserer_usine(racine->droite, id, vol_info, pourcentage_fuite, type_ligne);
    } else {
        if (type_ligne == TYPE_CAPACITE_MAX) {
            racine->capacite_max = vol_info;
        } else if (type_ligne == TYPE_SOURCE_USINE) {
            racine->volume_capte += vol_info; 
            long long volume_net_traite = vol_info *(1.0 -(pourcentage_fuite/100.0));
            racine->volume_traite += volume_net_traite;
        } 
        return racine;
    }

    return equilibrer_arbre(racine);
}

