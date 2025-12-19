#include <stdio.h>
#include "../Data/Data.h"

#include <stdlib.h>
#include <string.h>
#include "../Parser/Parser.h" 

NetworkComponent* find_or_create_component(NodeIndex** index_root, char* id, FacilityType type) {
    if (*index_root == NULL) {
        NetworkComponent* new_comp = malloc(sizeof(NetworkComponent));
        *index_root = malloc(sizeof(NodeIndex));
        return new_comp;
    }

    int cmp = strcmp(id, (*index_root)->id);
    NetworkComponent* res = NULL; 

    if (cmp < 0){
        res = find_or_create_component(&((*index_root)->left), id, type);
    } else if (cmp > 0){
        res = find_or_create_component(&((*index_root)->right), id, type);
    } else {
        return (*index_root)->component_ptr; // Déjà présent
    }

    (*index_root)->height = 1 + max(hauteur_node_index((*index_root)->left), hauteur_node_index((*index_root)->right));

    *index_root = equilibrer_arbre_index(*index_root);

    return res; 
}

void link_components(NetworkComponent* parent, NetworkComponent* child, double leak_p) {
    if (parent == NULL || child == NULL){
        return;
    }
    child->leak_percent = leak_p;
    
    // Add the child to the parent's list
    child->next_sibling = parent->first_child;
    parent->first_child = child;
}

double calculate_total_leaks(NetworkComponent* current, double incoming_volume) {
    if (current == NULL) {
        return 0.0;
    }

    double volume_after_leak = incoming_volume * (1.0 - (current->leak_percent / 100.0));
    double loss_in_this_segment = incoming_volume - volume_after_leak;

    // Count children to distribute the water equally
    int children_count = 0;
    NetworkComponent* child = current->first_child;
    while (child != NULL) {
        children_count++;
        child = child->next_sibling;
    }

    double downstream_losses = 0.0;
    if (children_count > 0) {
        double volume_per_child = volume_after_leak / children_count;
        
        child = current->first_child;
        while (child != NULL) {
            downstream_losses += calculate_total_leaks(child, volume_per_child);
            child = child->next_sibling;
        }
    }

    return loss_in_this_segment + downstream_losses;
}



void leaks(char* db_path, char* target_factory_id) {
    FILE* file = fopen(db_path, "r");
    if (file == NULL){
        return;
    }
    NodeIndex* index = NULL;
    NetworkComponent* root_factory = NULL;
    double total_captured_for_factory = 0.0;
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        Facility* f = parserLine(line); 
        if(f == NULL){
            continue;
        }

        NetworkComponent* parent = find_or_create_component(&index, f->parent_id, NONE);
        NetworkComponent* child = find_or_create_component(&index, f->id, f->type);

        link_components(parent, child, f->leak);

   
        if (strcmp(child->id, target_factory_id) == 0 && f->type == FACILITY_COMPLEX) {
            root_factory = child;
        }
        
        if (strcmp(f->parent_id, target_factory_id) != 0 && strcmp(f->id, target_factory_id) == 0) {
             total_captured_for_factory += f->volume_traite; 
        }

        free(f);
    }

    if (root_factory) {
        double lost_volume = calculate_total_leaks(root_factory, total_captured_for_factory);
        printf("Total volume lost for %s: %.3f M.m3\n", target_factory_id, lost_volume);
    } else {
        printf("-1\n"); 
    }

    fclose(file);
}


