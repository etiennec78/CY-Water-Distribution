#ifndef LEAKS_H
#define LEAKS_H

#include "../Data/Data.h"

void leaks(char* db_path, char* target_id);
void freeAll(NodeIndex* root);
NetworkComponent* find_in_index(NodeIndex* root, char* id);
NodeIndex* insert_index(NodeIndex** root, NetworkComponent* comp);
NetworkComponent* get_or_create(NodeIndex** index, char* id);
double calculate_recursive_volume(NetworkComponent* current, double volume_in);

#endif
