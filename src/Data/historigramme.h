#ifndef HISTORIGRAMME_H
#define HISTORIGRAMME_H

#include "structure.h"

Resultat_Histo *recup_histo_data(UsineNode* node,char* id);
UsineNode *recherche_usines_histo(UsineNode* node, char* id);
void parcours_inverse_et_ecriture(UsineNode* node, const char* nom_fichier, const char* type_metrique);

#endif

