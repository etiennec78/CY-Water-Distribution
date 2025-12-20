#ifndef LEAKS_H
#define LEAKS_H

#include "../Data/Data.h"

void leaks(char* db_path, char* target_id);
NodeIndex* chargerReseauLeaks(char* nom_fichier, char* target_id, double* total_vol_entrant);
NetworkComponent* find_or_create_component(NodeIndex** root, char* id);
double calculate_recursive_volume(NetworkComponent* current, double volume_in);
NetworkComponent* rechercher_composant_par_id(NodeIndex* root, char* target_id);
void freeAll(NodeIndex* root);

#endif