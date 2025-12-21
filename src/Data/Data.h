#ifndef DATA_H
#define DATA_H


typedef enum {
    UNKNOWN,
    SOURCE_TO_FACTORY,
    FACTORY_ONLY,
    FACTORY_TO_STORAGE,
    STORAGE_TO_CUST //Contain STORAGE_TO_JUNCTION & JUNCTION_TO_RACCORDEMENT & RACCORDEMENT_TO_CUST
} LineType;

typedef struct Facility {
    char id[50];
    char parent_id[50];
    LineType type_ligne;
    double volume;
    double leak;

    double capacite_max;
    double volume_traite;
    double volume_capte;
    double volume_perdu;
    
    struct Facility* droite;
    struct Facility* gauche;

    int equilibre;
} Facility;

typedef struct {
    char* id;
    double capacite_max; 
    double volume_traite;
    double volume_capte;
} Resultat_Histo;

// if more than 2 childs
typedef struct NetworkComponent {
    char id[50];
    double leak_percent;
    
    struct NetworkComponent* first_child;  
    struct NetworkComponent* next_sibling; // Child on the same level
} NetworkComponent;

typedef struct NodeIndex {
    char id[50];
    NetworkComponent* component_ptr; 
    struct NodeIndex *left;
    struct NodeIndex *right;
    int height;
} NodeIndex;


#endif
