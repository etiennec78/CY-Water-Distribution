#ifndef PARSER_H
#define PARSER_H
#include "../Data/Data.h"


int countLine();
FacilityType conversionCharToType(char* mot);
detectLineType(char* col1, char* col2, char* col3, char* col4, char* col5);
Facility* parserLine(char* lineStr);

#endif