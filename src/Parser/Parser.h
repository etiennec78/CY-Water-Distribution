#ifndef PARSER_H
#define PARSER_H
#include "../Data/Data.h"

#define COL_LEN 5
#define FACTORY_STRUCT_LEN 6

int countLine();
FacilityType conversionCharToType(char* mot);
LineType detectLineType(char** cols);
Facility* parserLine(char* lineStr);

#endif
