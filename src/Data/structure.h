#ifndef STRUCTURE_H
#define STRUCTURE_H




typedef struct UsineNode{ 
      char *id_usines;       
      long long capacite_max; 
      long long volume_traite;
      long long volume_capte;

      struct UsineNode* gauche; 
      struct UsineNode* droite;
      
      int hauteur; 
}UsineNode;

typedef struct Resultat_Histo{
    char* id;
    long long capacite_max; 
    long long volume_traite;
    long long volume_capte;
}Resultat_Histo;

#endif