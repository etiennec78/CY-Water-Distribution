#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leaks.h"
#include "../Data/Data.h"


void freeAll(NodeIndex* root) {
    if (root == NULL) return;

    freeAll(root->left);
    freeAll(root->right);
    if (root->component_ptr != NULL) {
        free(root->component_ptr);
    }
    free(root);
}
NetworkComponent* find_or_create_component(NodeIndex** root, char* id) {
    if (*root == NULL) {
        NetworkComponent* new_comp = malloc(sizeof(NetworkComponent));
        if (!new_comp) return NULL;
        strncpy(new_comp->id, id, 49);
        new_comp->id[49] = '\0';
        new_comp->leak_percent = 0;
        new_comp->first_child = NULL;
        new_comp->next_sibling = NULL;

        *root = malloc(sizeof(NodeIndex));
        if (!(*root)) { free(new_comp); return NULL; }
        strncpy((*root)->id, id, 49);
        (*root)->id[49] = '\0';
        (*root)->component_ptr = new_comp;
        (*root)->left = (*root)->right = NULL;
        (*root)->height = 1;
        return new_comp;
    }

    int cmp = strcmp(id, (*root)->id);
    if (cmp < 0) return find_or_create_component(&((*root)->left), id);
    if (cmp > 0) return find_or_create_component(&((*root)->right), id);
    return (*root)->component_ptr;
}


NetworkComponent* rechercher_composant_par_id(NodeIndex* root, char* target_id) {
    if (root == NULL) return NULL;
    if (strcmp(root->id, target_id) == 0) return root->component_ptr;
    
    NetworkComponent* res = rechercher_composant_par_id(root->left, target_id);
    if (res != NULL) return res;
    return rechercher_composant_par_id(root->right, target_id);
}

double calculate_recursive_volume(NetworkComponent* current, double volume_in) {
    if (!current || volume_in <= 0) return 0;

    double volume_out = volume_in * (1.0 - (current->leak_percent / 100.0));

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
        total_delivered += calculate_recursive_volume(child, volume_out / (double)count);
        child = child->next_sibling;
    }
    return total_delivered;
}

NodeIndex* chargerReseauLeaks(char* nom_fichier, char* target_id, double* total_vol_entrant) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL){
        return NULL;
    }
    char ligne[1024];
    NodeIndex* index_root = NULL;
    *total_vol_entrant = 0.0;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        ligne[strcspn(ligne, "\r\n")] = 0;
        if (strlen(ligne) < 5) continue;

        char* tmp = strdup(ligne);
        char* cols[6] = {NULL};
        int i = 0;
        char* token = strtok(tmp, ";");
        
        while (token && i < 6) {
            cols[i++] = token;
            token = strtok(NULL, ";");
        }

        if (i >= 4) {
            char* id_parent = cols[1];
            char* id_enfant = cols[2];
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
                *total_vol_entrant += vol;
            }
            else if (strcmp(id_enfant, target_id) == 0) {
                *total_vol_entrant += vol;
            }

            if (strcmp(id_parent, "-") != 0 && strcmp(id_enfant, "-") != 0) {
                NetworkComponent* p = find_or_create_component(&index_root, id_parent);
                NetworkComponent* c = find_or_create_component(&index_root, id_enfant);
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
    fclose(fichier);
    return index_root;
}

void leaks(char* db_path, char* target_id) {
    double vol_depart = 0;
    NodeIndex* index = chargerReseauLeaks(db_path, target_id, &vol_depart);

    NetworkComponent* start = rechercher_composant_par_id(index, target_id);

    if (start && vol_depart > 0) {
        double vol_final = calculate_recursive_volume(start, vol_depart);
        double total_perdu = (vol_depart - vol_final);

        FILE* out = fopen("data/leaks.dat", "a");
        if (out) {
            fprintf(out, "%s;%.3f M.m3\n", start->id, total_perdu / 1000.0);
            fclose(out);
        }
        
        printf("--- Resultats Fuites ---\n");
        printf("ID : %s\n", start->id);
        printf("Volume initial : %.2f\n", vol_depart);
        printf("Volume final : %.2f\n", vol_final);
        // Ajout de l'unité sur le terminal
        printf("Pertes totales : %.3f M.m3\n", total_perdu / 1000.0);
    } else {
        if (start == NULL) {
            printf("Erreur : L'ID '%s' n'existe pas.\n", target_id);
        } else {
            printf("Erreur : Volume nul pour '%s'.\n", target_id);
        }
    }
    freeAll(index);
}
