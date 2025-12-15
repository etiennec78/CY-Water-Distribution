#include <stdio.h>

void leaks(char* db_path, char* factory_id) {
    printf("DB: %s, factory: %s\n", db_path, factory_id);
}