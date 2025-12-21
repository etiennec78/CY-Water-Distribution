#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leaks.h"
#include "../Data/Data.h"
#include "../Parser/Parser.h"

static void free_tree_recursive(NetworkComponent* node) {
    if (node == NULL) {
        return;
    }
    NetworkComponent* child = node->first_child;
    while (child != NULL) {
        NetworkComponent* next = child->next_sibling;
        free_tree_recursive(child);
        child = next;
    }
    free(node);
}

void freeAll(NodeIndex* root) {
    if (root == NULL) {
        return;
    }
    freeAll(root->left);
    freeAll(root->right);
    free(root);
}

NetworkComponent* find_in_index(NodeIndex* root, char* id) {
    if (root == NULL) {
        return NULL;
    }
    int cmp = strcmp(id, root->id);
    if (cmp < 0) {
        return find_in_index(root->left, id);
    } else if (cmp > 0) {
        return find_in_index(root->right, id);
    }
    return root->component_ptr;
}

NodeIndex* insert_index(NodeIndex** root, NetworkComponent* comp) {
    if (*root == NULL) {
        *root = malloc(sizeof(NodeIndex));
        if (*root == NULL) return NULL;
        strncpy((*root)->id, comp->id, 49);
        (*root)->id[49] = '\0';
        (*root)->component_ptr = comp;
        (*root)->left = (*root)->right = NULL;
        return *root;
    }
    int cmp = strcmp(comp->id, (*root)->id);
    if (cmp < 0) {
        return insert_index(&((*root)->left), comp);
    } else if (cmp > 0) {
        return insert_index(&((*root)->right), comp);
    }
    return *root;
}

NetworkComponent* get_or_create(NodeIndex** index, char* id) {
    NetworkComponent* found = find_in_index(*index, id);
    if (found != NULL) {
        return found;
    }
    NetworkComponent* new_comp = malloc(sizeof(NetworkComponent));
    if (new_comp == NULL) return NULL;
    strncpy(new_comp->id, id, 49);
    new_comp->id[49] = '\0';
    new_comp->leak_percent = 0.0;
    new_comp->first_child = NULL;
    new_comp->next_sibling = NULL;
    insert_index(index, new_comp);
    return new_comp;
}

double calculate_recursive_volume(NetworkComponent* current, double volume_in) {
    if (current == NULL || volume_in <= 0) {
        return 0;
    }
    double volume_out = volume_in * (1.0 - (current->leak_percent / 100.0));
    if (current->first_child == NULL) {
        return volume_out;
    }
    int count = 0;
    NetworkComponent* child = current->first_child;
    while (child != NULL) {
        count++;
        child = child->next_sibling;
    }
    double total_delivered = 0;
    child = current->first_child;
    while (child != NULL) {
        total_delivered += calculate_recursive_volume(child, volume_out / (double)count);
        child = child->next_sibling;
    }
    return total_delivered;
}

void leaks(char* db_path, char* target_id) {
    FILE* fichier = fopen(db_path, "r");
    if (fichier == NULL) {
        return;
    }
    NodeIndex* index = NULL;
    double vol_entree_usine = 0.0;
    char ligne[1024];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        char ligne_copy[1024];
        strcpy(ligne_copy, ligne);
        char* cols[5];
        char* ptr = ligne_copy;
        for (int i = 0; i < 5; i++) {
            cols[i] = strsep(&ptr, ";\n\r");
        }
        LineType type = detectLineType(cols);
        if (type == SOURCE_TO_FACTORY && strcmp(cols[2], target_id) == 0) {
            vol_entree_usine += atof(cols[3]);
        }
        if (type == FACTORY_TO_STORAGE || type == STORAGE_TO_CUST) {
            NetworkComponent* amont = get_or_create(&index, cols[1]);
            NetworkComponent* aval = get_or_create(&index, cols[2]);
            if (cols[4] != NULL && strcmp(cols[4], "-") != 0) {
                aval->leak_percent = atof(cols[4]);
            }
            int exists = 0;
            NetworkComponent* check = amont->first_child;
            while (check != NULL) {
                if (check == aval) {
                    exists = 1;
                    break;
                }
                check = check->next_sibling;
            }
            if (exists == 0) {
                aval->next_sibling = amont->first_child;
                amont->first_child = aval;
            }
        }
    }
    fclose(fichier);
    NetworkComponent* start = find_in_index(index, target_id);
    if (start != NULL && vol_entree_usine > 0) {
        double vol_final = calculate_recursive_volume(start, vol_entree_usine);
        double total_perdu = (vol_entree_usine - vol_final) / 1000.0;
        FILE* out = fopen("data/leaks.dat", "a");
        if (out != NULL) {
            fprintf(out, "%s;%.3f\n", start->id, total_perdu);
            fclose(out);
        }
        printf("%s;%.3f M.m3\n", start->id, total_perdu);
    } else {
        FILE* out = fopen("data/leaks.dat", "a");
        if (out != NULL) {
            fprintf(out, "%s;-1\n", target_id);
            fclose(out);
        }
    }
    if (start != NULL) {
        free_tree_recursive(start);
    }
    freeAll(index);
}
