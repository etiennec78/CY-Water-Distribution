#ifndef AVL_HISTO_H
#define AVL_HISTO_H

#include "common.h"

void free_avl_facility(Facility *root);
Facility *insert_facility(Facility *root, char *id, double vol_info, double leak_percentage, LineType line_type, int *h);
Facility* create_avl_max(char* filename);

#endif
