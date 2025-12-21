#ifndef PARSER_H
#define PARSER_H
#include "../Data/common.h"

#define COL_LEN 5
#define COL_CHECK_LEN 3
#define FACTORY_STRUCT_LEN 6

Facility* parse_line(char* lineStr, Facility* factory_tree);

#endif
