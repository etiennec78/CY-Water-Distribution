#ifndef PARSER_H
#define PARSER_H
#include "../Data/Data.h"

Capture* parserTabCapture();
Factory* parserTabFactory();
Storage* parserTabStorage();
connexion* parserTabConnexion(); 
principalDistribution* parserTabPrincipalDistribution();
secondaryDistribution* parserTabSecondaryDistribution();  
int countLine();
FacilityType conversionCharToType(char* mot);

#endif