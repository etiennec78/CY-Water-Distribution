#ifndef USINE_AVL_H
#define USINE_AVL_H

#include "Data.h"

NetworkComponent* find_or_create_component(NodeIndex** root, char* id);
NetworkComponent* rechercher_composant_par_id(NodeIndex* root, char* target_id);
void free_avl_leaks(NodeIndex* root);

#endif
