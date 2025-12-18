#include <stdio.h>
#include <string.h>
#include "Histogramme/histogramme.h"
#include "Leaks/leaks.h"

int main(int argc, char **argv) {

    if (strcmp(argv[2], "histo") == 0) {
        histogramme(argv[1], argv[3]);
    }
    else if (strcmp(argv[2], "leaks") == 0) {
        leaks(argv[1], argv[3]);
    }

    return 0;
}
