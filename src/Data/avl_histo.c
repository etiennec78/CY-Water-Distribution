#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "../Parser/parser.h"

#define TYPE_CAPACITE_MAX 1
#define TYPE_SOURCE_USINE 2
#define TYPE_USINE_STOCKAGE 3

Facility *left_rotate_facility(Facility *avl) {
    Facility *pivot = avl->right;
    
    avl->right = pivot->left;
    pivot->left = avl;

    int balanceAVL = avl->balance;
    int balancePivot = pivot->balance;

    // Update balance factors after rotaton
    avl->balance = balanceAVL - max(balancePivot, 0) - 1;
    pivot->balance = min(min(balanceAVL - 2, balanceAVL + balancePivot - 2), balancePivot - 1);

    return pivot;
}

Facility *right_rotate_facility(Facility *avl) {
    Facility *pivot = avl->left;
    
    avl->left = pivot->right;
    pivot->right = avl;

    int balanceAVL = avl->balance;
    int balancePivot = pivot->balance;

    // Update balance factors after rotation
    avl->balance = balanceAVL - max(balancePivot, 0) + 1;
    pivot->balance = max(max(balanceAVL + 2, balanceAVL + balancePivot + 2), balancePivot + 1);

    return pivot;
}

Facility *right_left_rotate_facility(Facility *n) {
    n->right = right_rotate_facility(n->right);
    return left_rotate_facility(n);
}

Facility *left_right_rotate_facility(Facility *n) {
    n->left = left_rotate_facility(n->left);
    return right_rotate_facility(n);
}

Facility *balance_tree(Facility *avl) {
    if (avl == NULL) return avl;

    // Check if the tree is unbalanced
    if (avl->balance > 1) {
        if (avl->right->balance >= 0) {
            return left_rotate_facility(avl);
        } else {
            return right_left_rotate_facility(avl);
        }
    } else if (avl->balance < -1) {
        if (avl->left->balance <= 0) { 
            return right_rotate_facility(avl);
        } else {
            return left_right_rotate_facility(avl);
        }
    }
    return avl;
}

Facility *new_facility(char *id) {
    Facility *new_node = malloc(sizeof(Facility));
    if (new_node == NULL) {
        return NULL;
    }
    
    strncpy(new_node->id, id, 49); 
    new_node->id[49] = '\0';

    new_node->volume = 0.0;
    new_node->leak = 0.0;
    new_node->parent_id[0] = '\0';
    new_node->line_type = UNKNOWN;
    
    new_node->max_capacity = 0.0;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->treated_volume = 0.0;
    new_node->captured_volume = 0.0;
    new_node->balance = 0;

    return new_node;
}

void update_facility_values(Facility *node, double vol_info, double leak_percentage, LineType line_type) {
    if (line_type == FACTORY_ONLY) {
        node->max_capacity = vol_info / 1000.0;
        node->line_type = FACTORY_ONLY;
    } else if (line_type == SOURCE_TO_FACTORY) {
        // Accumulate captured volume from source
        node->captured_volume += vol_info / 1000.0;
        // Calculate net treated volume
        double treated_volume = (vol_info / 1000.0) * (1.0 - (leak_percentage / 100.0));
        node->treated_volume += treated_volume;
    }
    // FACTORY_TO_STORAGE: do nothing
}

Facility *insert_facility(Facility *root, char *id, double vol_info, double leak_percentage, LineType line_type, int *h) {
    if (root == NULL) {
        *h = 1;
        root = new_facility(id);
        if (root != NULL) {
            update_facility_values(root, vol_info, leak_percentage, line_type);
        }
        return root;
    }

    int cmp = strcmp(id, root->id);

    if (cmp < 0) {
        root->left = insert_facility(root->left, id, vol_info, leak_percentage, line_type, h);
    } else if (cmp > 0) {
        root->right = insert_facility(root->right, id, vol_info, leak_percentage, line_type, h);
    } else {
        update_facility_values(root, vol_info, leak_percentage, line_type);
        *h = 0;
        return root;
    }

    if (cmp < 0) *h = -*h;
    
    if (*h != 0) {
        root->balance = root->balance + *h;
        root = balance_tree(root);

        if (root->balance == 0) {
            *h = 0;
        } else {
            *h = 1;
        }
    }

    return root;
}

void free_avl_facility(Facility* root) {
    if (root ==NULL) {
        return;
    }
    free_avl_facility(root->left);
    free_avl_facility(root->right);
    free(root);
}

 
 Facility* create_avl_max(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erreur ouverture fichier");
        return NULL;
    }
    
    char line[500];
    Facility* factory_tree = NULL;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        factory_tree = parse_line(line, factory_tree);
    }

    fclose(file);
    return factory_tree;
}
