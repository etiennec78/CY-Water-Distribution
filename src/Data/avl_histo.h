#ifndef AVL_HISTO_H
#define AVL_HISTO_H

#include "common.h"

void free_avl_usine(Facility *racine);
Facility *inserer_usine(Facility *racine, char *id, double vol_info, double pourcentage_fuite, LineType type_ligne, int *h);
Facility* creerAVLMax(char* nom_fichier);

#endif
