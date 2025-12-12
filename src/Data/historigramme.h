#ifndef HISTORIGRAMME_H
#define HISTORIGRAMME_H

#include "Data.h"

Resultat_Histo *recup_histo_data(Facility* node,char* id);
Facility *recherche_usines_histo(Facility* node, char* id);
void parcours_inverse_et_ecriture(Facility* node, const char* nom_fichier, const char* type_metrique);

#endif

