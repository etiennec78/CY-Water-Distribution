#ifndef USINE_AVL_H
#define USINE_AVL_H

#include "Data.h"

void free_avl_usine(Facility *racine);
Facility *inserer_usine(Facility *racine, char *id, double vol_info, double pourcentage_fuite, LineType type_ligne, int *h);
Facility* creerAVLMax(char* nom_fichier);

#endif
