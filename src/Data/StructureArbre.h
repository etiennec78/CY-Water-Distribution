#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct UsineNode{ // AVL des usines
      char *id_usines;       //clé de recherche (identifiant de l'usine)
      long long capacite_max; // utilisations de long long parce que les quantités sont longues et on évite l'overflow
      long long volume_capte;
      long long volume_traite;

      struct UsineNode* gauche; // pointeur gauche
      struct UsineNode* droite; // pointeur droite

      int hauteur; 
}UsineNode;


typedef struct EnfantListe{ // Liste des enfants
    struct ReseauAval *enfant;
    struct EnfantListe *suivant;   
}EnfantListe;

typedef struct ReseauAval{ //structure Arbre les acteurs du réseau aval de l'usine
    char *id_acteur; //clé de recherche
    long long volume_entrant;
    float pourcentage_fuite_amont;
    int nb_enfant;

    struct EnfantListe* enfant; //pointeur vers le début de la liste chainée des enfants
}ReseauAval;

typedef struct IndexNode{ //structure pour l'AVL pour les noeuds du réseau
    char *id_acteur; //clé de recherche
    struct ReseauAval *adresse_acteur;
    struct IndexNode* gauche;
    struct IndexNode* droit;

    int hauteur;
}IndexNode;

#endif
