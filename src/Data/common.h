#ifndef COMMON_H
#define COMMON_H


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
    LineType line_type;
    double volume;
    double leak;

    double max_capacity;
    double treated_volume;
    double captured_volume;
    double lost_volume;
    
    struct Facility* right;
    struct Facility* left;

    int balance;
} Facility;

typedef struct {
    char* id;
    double max_capacity; 
    double treated_volume;
    double captured_volume;
} HistoResult;

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


int max(int a, int b);
int min(int a, int b);


#endif
