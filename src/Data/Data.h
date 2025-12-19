#ifndef DATA_H
#define DATA_H

typedef enum {
    NONE,
    SOURCE,
    SPRING,
    POND,
    CATCHMENT,
    WELL,
    RESURGENCE,
    INTAKE,
    BOREHOLE,
    FOUNTAIN,
    RESERVOIR,
    FORAGE,
    WELL_FIELD,
    TANK,
    UNIT,
    MODULE,
    FACILITY_COMPLEX,
    STORAGE,
    JUNCTION,
    CUST,
    TYPE_UNKNOWN ,
} FacilityType;

typedef enum {
    UNKNOWN,
    SOURCE_TO_FACTORY,
    FACTORY_ONLY,
    FACTORY_TO_STORAGE,
    STORAGE_TO_JUNCTION,
    JUNCTION_TO_SERVICE,
    SERVICE_TO_CUST
} LineType;

typedef struct Facility{
    FacilityType type;
    char id[50];
    char parent_id[50];
    double volume;
    double leak;

    double capacite_max;
    double volume_traite;
    double volume_capte;
    double volume_perdu;
    
    struct Facility* droite;
    struct Facility* gauche;

    int hauteur;
}Facility;

typedef struct Resultat_Histo{
    char* id;
    double capacite_max; 
    double volume_traite;
    double volume_capte;
}Resultat_Histo;

// if more than 2 childs
typedef struct NetworkComponent {
    FacilityType type;
    char id[50];
    double leak_percent;
    
    struct NetworkComponent* first_child;  
    struct NetworkComponent* next_sibling; // Child on the same level
} NetworkComponent;






#endif
