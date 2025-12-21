#ifndef AVL_LEAKS_H
#define AVL_LEAKS_H

#include "common.h"

NetworkComponent* find_or_create_component(NodeIndex** root, char* id);
NetworkComponent* find_component_by_id(NodeIndex* root, char* target_id);
void free_avl_leaks(NodeIndex* root);

#endif
