#include "../Data/common.h"

typedef double (*capacity_getter)(Facility* f);

typedef struct {
    capacity_getter* list;
    int length;
} GetterList;

void histogram(char* db_path, char* histo_type);
