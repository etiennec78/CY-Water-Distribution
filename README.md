# C-WildWater Project 🌊

## Description 📝

Ce projet permet de faire la synthèse de données d’un système de distribution d’eau à partir d'un fichier de données.
Il utilise un script Shell pour l'interface utilisateur, un programme C pour le traitement des données et un script Python pour la création d'histogrammes.

## Compilation ⚙️

Le projet utilise un Makefile pour la compilation.
Le script [cwildwater.sh](cwildwater.sh) se charge automatiquement de compiler le programme si nécessaire.

## Utilisation 🛠️

Le point d'entrée est le script [cwildwater.sh](cwildwater.sh).

Syntaxe :
    ./cwildwater.sh <fichier_donnees> [arg] [sous_arg]

Arguments disponibles :

#### Histogrammes (histo) 📊
   Génère des histogrammes des usines de traitement.
   
   Utilisation : `./cwildwater.sh <fichier_donnees> histo [type]`
   
   Types disponibles :
   - max  : Capacité maximale de traitement
   - src  : Volume total capté par les sources
   - real : Volume total réellement traité
   - all  : Histogramme cumulant les 3 types

   Exemple :
   `./cwildwater.sh data/c-wildwater_v3.dat histo all`

   Les résultats sont générés dans le dossier `data/` sous forme de fichiers .dat et d'images .png.

#### Fuites (leaks) 💧
   Calcule les pertes d'eau pour une usine donnée sur l'ensemble de son réseau aval.

   Utilisation : `./cwildwater.sh <fichier_donnees> leaks "<id_usine>"`

   Exemple :
   `./cwildwater.sh data/c-wildwater_v3.dat leaks "Facility complex #RH400057F"`

   Les résultats sont affichés dans le terminal et ajoutés au fichier `data/leaks.dat`.
