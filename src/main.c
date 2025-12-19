#include <stdio.h>
#include <string.h>
#include "Histogramme/histogramme.h"
#include "Leaks/leaks.h"

int main(int argc, char *argv[]) {

    if (argc < 4) {
        return 1; 
    }

    char *db_path = argv[1];
    char *mode = argv[2];
    char *sub_arg = argv[3];

    if (strcmp(mode, "histo") == 0) {
        histogramme(db_path, sub_arg);
    } 

    else if (strcmp(mode, "leaks") == 0) {
        leaks(db_path, sub_arg);
    } 
    else {
        return 1;
    }

    return 0;
}


    
    
