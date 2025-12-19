#include <stdio.h>
#include "../Data/Data.h"

void leaks(char* db_path, char* factory_id) {
    printf("DB: %s, factory: %s\n", db_path, factory_id);
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