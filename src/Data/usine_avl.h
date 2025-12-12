#ifndef USINE_AVL_H
#define USINE_AVL_H

#include "Data.h"

void free_avl_usine(Facility *racine);
int max(int a, int b);
int hauteur_usine(Facility *node);
int equilibrage(Facility *node);
Facility *rotation_droite_usine(Facility *y);
Facility *rotation_gauche_usine(Facility *x);
Facility *rotation_gauche_droite_usine(Facility *z);
Facility *rotation_droite_gauche_usine(Facility *z);
Facility *nouvelle_usine(char *id, double capacite_max);
Facility *inserer_usine(Facility *racine, char *id, double vol_info, double pourcentage_fuite, int type_ligne);



#endif