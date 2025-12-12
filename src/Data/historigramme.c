#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usine_avl.h"
#include "structure.h"
#include "historigramme.h"



UsineNode* recherche_usines_histo(UsineNode* node, char* id){
    if(node == NULL){
        return NULL;
    }
    int comparaison = strcmp(id,node->id_usines);
    
    if(comparaison<0){
        return recherche_usines_histo(node->gauche,id);
    }
    else if(comparaison>0){
        return recherche_usines_histo(node->droite,id);
    }
    else{
        return node;
    }
}

Resultat_Histo* recup_histo_data(UsineNode* node, char* id){
    UsineNode* usine_trouvee = recherche_usines_histo(node, id);
    
    if(usine_trouvee == NULL){
        return NULL;
    }
    Resultat_Histo* data = malloc(sizeof(Resultat_Histo));
    if(data == NULL){
        return NULL;
    }

    data->id = strdup(id);
    if(data->id == NULL){
        free(data);
        return NULL;
    }

    data->capacite_max = usine_trouvee->capacite_max;
    data->volume_capte = usine_trouvee->volume_capte;
    data->volume_traite = usine_trouvee->volume_traite;

    return data;
}

void parcours_inverse_et_ecriture(UsineNode* node, FILE* fichier, const char* type_metrique) {
    if (node != NULL) {
       
        parcours_inverse_et_ecriture(node->droite, fichier, type_metrique);

        
        long long volume_km3 = 0;
        if (strcmp(type_metrique, "max") == 0) {
            volume_km3 = node->capacite_max;
        } else if (strcmp(type_metrique, "src") == 0) {
            volume_km3 = node->volume_capte;
        } else if (strcmp(type_metrique, "real") == 0) {
            volume_km3 = node->volume_traite;
        }

        
        double volume_mm3 = (double)volume_km3 / 1000.0;

        fprintf(fichier, "%s;%.3f\n", node->id_usines, volume_mm3); 

        parcours_inverse_et_ecriture(node->gauche, fichier, type_metrique);
    }
}









