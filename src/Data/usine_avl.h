#ifndef USINE_AVL_H
#define USINE_AVL_H

#include "structure.h"

void free_avl_usine(UsineNode *racine);
int max(int a, int b);
int hauteur_usine(UsineNode *node);
int equilibre(UsineNode *node);
UsineNode *rotation_droite_usine(UsineNode *y);
UsineNode *rotation_gauche_usine(UsineNode *x);
UsineNode *rotation_gauche_droite_usine(UsineNode *z);
UsineNode *rotation_droite_gauche_usine(UsineNode *z);
UsineNode *nouvelle_usine(char *id, long long capacite_max);
UsineNode *inserer_usine(UsineNode *racine, char *id, long long vol_info, float pourcentage_fuite, int type_ligne);

#endif