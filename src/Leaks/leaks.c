#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leaks.h"
#include "../Data/common.h"
#include "../Data/avl_leaks.h"

typedef struct {
    double max_leak_volume;
    char max_leak_id_upstream[50];
    char max_leak_id_downstream[50];
} LeakStats;

double calculate_recursive_volume(NetworkComponent* current, double volume_in, char* parent_id, LeakStats* stats) {
    if (!current || volume_in <= 0) return 0;

    double leak_amount = volume_in * (current->leak_percent / 100.0);
    double volume_out = volume_in - leak_amount;

    if (stats && parent_id && leak_amount > stats->max_leak_volume) {
        stats->max_leak_volume = leak_amount;
        snprintf(stats->max_leak_id_upstream, 50, "%s", parent_id);
        snprintf(stats->max_leak_id_downstream, 50, "%s", current->id);
    }

    if (current->first_child == NULL) {
        return volume_out;
    }

    int count = 0;
    NetworkComponent* child = current->first_child;
    while (child) {
        count++;
        child = child->next_sibling;
    }

    double total_delivered = 0;
    child = current->first_child;
    while (child) {
        total_delivered += calculate_recursive_volume(child, volume_out / (double)count, current->id, stats);
        child = child->next_sibling;
    }
    return total_delivered;
}

NodeIndex* load_leaks_network(char* filename, char* target_id, double* total_vol_in) {
    FILE* file = fopen(filename, "r");
    if (file == NULL){
        return NULL;
    }
    char line[1024];
    NodeIndex* index_root = NULL;
    *total_vol_in = 0.0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) < 5) continue;

        char* tmp = strdup(line);
        char* cols[6] = {NULL};
        int i = 0;
        char* token = strtok(tmp, ";");
        
        while (token && i < 6) {
            cols[i++] = token;
            token = strtok(NULL, ";");
        }

        if (i >= 4) {
            char* id_parent = cols[1];
            char* id_child = cols[2];
            double vol;
            if (strcmp(cols[3], "-") != 0) {
                vol = atof(cols[3]);
            } else {
                vol = 0.0;
            }

            double leak;
            if (i >= 5 && strcmp(cols[4], "-") != 0) {
                leak = atof(cols[4]);
            } else {
                leak = 0.0;
            }

            if (strcmp(id_parent, target_id) == 0) {
                *total_vol_in += vol;
            }
            else if (strcmp(id_child, target_id) == 0) {
                *total_vol_in += vol;
            }

            if (strcmp(id_parent, "-") != 0 && strcmp(id_child, "-") != 0) {
                NetworkComponent* p = find_or_create_component(&index_root, id_parent);
                NetworkComponent* c = find_or_create_component(&index_root, id_child);
                c->leak_percent = leak;

                int exists = 0;
                NetworkComponent* check = p->first_child;
                while(check) {
                    if (check == c) { exists = 1; break; }
                    check = check->next_sibling;
                }
                if (!exists) {
                    c->next_sibling = p->first_child;
                    p->first_child = c;
                }
            }
        }
        free(tmp);
    }
    fclose(file);
    return index_root;
}

void leaks(char* db_path, char* target_id) {
    double start_vol = 0;
    NodeIndex* index = load_leaks_network(db_path, target_id, &start_vol);

    NetworkComponent* start = find_component_by_id(index, target_id);

    int add_header = 0;
    FILE* check = fopen("data/leaks.dat", "r");
    if (check == NULL) {
        add_header = 1;
    } else {
        fseek(check, 0, SEEK_END);
        if (ftell(check) == 0) add_header = 1;
        fclose(check);
    }

    FILE* out = fopen("data/leaks.dat", "a");
    if (out) {
        if (add_header) {
            fprintf(out, "identifier;Leak volume (M.m3.year-1)\n");
        }
    }

    if (start) {
        LeakStats stats = {0, "", ""};
        double final_vol = calculate_recursive_volume(start, start_vol, NULL, &stats);
        double total_lost = (start_vol - final_vol) / 1000.0;

        fprintf(out, "%s;%.3f\n", start->id, total_lost);
        
        printf("--- Resultats Fuites ---\n");
        printf("ID : %s\n", start->id);
        printf("Volume initial : %.2f M.m3\n", start_vol / 1000.0);
        printf("Volume final : %.2f M.m3\n", final_vol / 1000.0);
        printf("Pertes totales : %.3f M.m3\n", total_lost);

        if (stats.max_leak_volume > 0) {
            printf("\n--- Pire fuite ---\n");
            printf("Tronçon : %s -> %s\n", stats.max_leak_id_upstream, stats.max_leak_id_downstream);
            printf("Volume perdu : %.3f M.m3\n", stats.max_leak_volume / 1000.0);
        }
    } else {
        printf("Erreur : L'ID '%s' n'existe pas.\n", target_id);
        fprintf(out, "%s;-1\n", target_id);
    }
    fclose(out);
    free_avl_leaks(index);
}
