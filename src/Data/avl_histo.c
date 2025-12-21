#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "../Parser/parser.h"

#define TYPE_CAPACITE_MAX 1 
#define TYPE_SOURCE_USINE 2 
#define TYPE_USINE_STOCKAGE 3

Facility *rotation_gauche_usine(Facility *avl) {
    Facility *pivot = avl->droite;
    
    avl->droite = pivot->gauche;
    pivot->gauche = avl;

    int equilibreAVL = avl->equilibre;
    int equilibrePivot = pivot->equilibre;

    avl->equilibre = equilibreAVL - max(equilibrePivot, 0) - 1;
    pivot->equilibre = min(min(equilibreAVL - 2, equilibreAVL + equilibrePivot - 2), equilibrePivot - 1);

    return pivot;
}

Facility *rotation_droite_usine(Facility *avl) {
    Facility *pivot = avl->gauche;
    
    avl->gauche = pivot->droite;
    pivot->droite = avl;

    int equilibreAVL = avl->equilibre;
    int equilibrePivot = pivot->equilibre;

    avl->equilibre = equilibreAVL - max(equilibrePivot, 0) + 1;
    pivot->equilibre = max(max(equilibreAVL + 2, equilibreAVL + equilibrePivot + 2), equilibrePivot + 1);

    return pivot;
}

Facility *rotation_droite_gauche_usine(Facility *n) {
    n->droite = rotation_droite_usine(n->droite);
    return rotation_gauche_usine(n);
}

Facility *rotation_gauche_droite_usine(Facility *n) {
    n->gauche = rotation_gauche_usine(n->gauche);
    return rotation_droite_usine(n);
}

Facility *equilibrer_arbre(Facility *avl) {
    if (avl == NULL) return avl;

    if (avl->equilibre > 1) { 
        if (avl->droite->equilibre >= 0) {
            return rotation_gauche_usine(avl);
        } else { 
            return rotation_droite_gauche_usine(avl);
        }
    } else if (avl->equilibre < -1) { 
        if (avl->gauche->equilibre <= 0) { 
            return rotation_droite_usine(avl);
        } else { 
            return rotation_gauche_droite_usine(avl);
        }
    }
    return avl;
}

Facility *nouvelle_usine(char *id) {
    Facility *new_node = malloc(sizeof(Facility));
    if (new_node == NULL) {
        return NULL;
    }
    
    strncpy(new_node->id, id, 49); 
    new_node->id[49] = '\0';

    new_node->volume = 0.0;
    new_node->leak = 0.0;
    new_node->parent_id[0] = '\0';
    new_node->type_ligne = UNKNOWN;
    
    new_node->capacite_max = 0.0;
    new_node->gauche = NULL;
    new_node->droite = NULL;
    new_node->volume_traite = 0.0;
    new_node->volume_capte = 0.0;
    new_node->equilibre = 0;
    
    return new_node;
}

void update_usine_values(Facility *node, double vol_info, double pourcentage_fuite, LineType type_ligne) {
    if (type_ligne == FACTORY_ONLY) {
        node->capacite_max = vol_info / 1000.0;
        node->type_ligne = FACTORY_ONLY; // Mark as factory
    } else if (type_ligne == SOURCE_TO_FACTORY) {
        node->volume_capte += vol_info / 1000.0;
        double volume_net_traite = (vol_info / 1000.0) * (1.0 - (pourcentage_fuite / 100.0));
        node->volume_traite += volume_net_traite;
    }
    // FACTORY_TO_STORAGE: do nothing
}

Facility *inserer_usine(Facility *racine, char *id, double vol_info, double pourcentage_fuite, LineType type_ligne, int *h) {
    if (racine == NULL) {
        *h = 1;
        racine = nouvelle_usine(id);
        if (racine != NULL) {
            update_usine_values(racine, vol_info, pourcentage_fuite, type_ligne);
        }
        return racine;
    }

    int cmp = strcmp(id, racine->id);

    if (cmp < 0) {
        racine->gauche = inserer_usine(racine->gauche, id, vol_info, pourcentage_fuite, type_ligne, h);
    } else if (cmp > 0) {
        racine->droite = inserer_usine(racine->droite, id, vol_info, pourcentage_fuite, type_ligne, h);
    } else {
        update_usine_values(racine, vol_info, pourcentage_fuite, type_ligne);
        *h = 0;
        return racine;
    }

    if (cmp < 0) *h = -*h;
    
    if (*h != 0) {
        racine->equilibre = racine->equilibre + *h;
        racine = equilibrer_arbre(racine);

        if (racine->equilibre == 0) {
            *h = 0;
        } else {
            *h = 1;
        }
    }

    return racine;
}

void free_avl_usine(Facility* racine) {
    if (racine ==NULL) {
        return;
    }
    free_avl_usine(racine->gauche);
    free_avl_usine(racine->droite);
    free(racine);
}

 
 Facility* creerAVLMax(char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        perror("Erreur ouverture fichier");
        return NULL;
    }
    
    char ligne[500];
    Facility* arbre_usines = NULL;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        ligne[strcspn(ligne, "\n")] = '\0';

        arbre_usines = parserLine(ligne, arbre_usines);
    }

    fclose(fichier);
    return arbre_usines;
}
