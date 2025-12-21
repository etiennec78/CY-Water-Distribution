#ifndef PARSER_H
#define PARSER_H
#include "../Data/common.h"

#define COL_LEN 5
#define COL_CHECK_LEN 3
#define FACTORY_STRUCT_LEN 6

Facility* parserLine(char* lineStr, Facility* arbre_usines);

#endif
