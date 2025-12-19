#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Data.h"
#include "histogramme.h"



Facility* recherche_usines_histo(Facility* node, char* id_search){
    if(node == NULL){
        return NULL;
    }
    int comparaison = strcmp(node->id,id_search);
    
    if(comparaison < 0){
        return recherche_usines_histo(node->gauche,id_search);
    }
    else if(comparaison > 0){
        return recherche_usines_histo(node->droite,id_search);
    }
    else{
        return node;
    }
}

Resultat_Histo* recup_histo_data(Facility* node, char* id_search){
    Facility* usine_trouve = recherche_usines_histo(node, id_search);
    
    if(usine_trouve == NULL){
        return NULL;
    }
    Resultat_Histo* data = malloc(sizeof(Resultat_Histo));
    if(data == NULL){
        return NULL;
    }

    data->id = strdup(id_search);
    if(data->id == NULL){
        free(data);
        return NULL;
    }

    data->capacite_max = usine_trouve->capacite_max;
    data->volume_capte = usine_trouve->volume_capte;
    data->volume_traite = usine_trouve->volume_traite;

    return data;
}
