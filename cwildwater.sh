if [ $# -lt 1 ]; then
    echo "Erreur: Pas assez d'arguments !"
    exit 1
fi

help_message="""
C-WildWaters: Un script pour effectuer un histogramme
des usines de traitement d'eau à partir d'un fichier .dat.

Arguments:
    histo [arg]: Créé un histogramme à partir des arguments suivants:
        max: Volume maximal de traitement de l'usine
        src: Volume total capté par les sources
        real: Volume total réellement traité
    leaks <id_usine>:
        Effectue la mesure des pertes d'une usine sur l'ensemble de son réseau aval.
        L'identifiant d'usine doit être complet et respecter la casse.
    help:
        Afficher ce message
"""

# Check that the first argument is valid
case "$1" in ("--help"|"help"|"histo"|"leaks")
    ;;
    *)
        echo "$help_message"
        echo "Erreur: Votre premier argument n'est pas valide ! "
        exit 2
    ;;
esac

# Print the help message if requested
for arg in $@; do
    if [[ "$arg" == "--help" || "arg" == "help" ]]; then
        echo "$help_message"
        exit 0
    fi
done

# Check that the makefile is present
if [ ! -f Makefile ]; then
    echo "Erreur: Impossible de trouver le makefile !"
    exit 3
fi

# Check that the src dir is present
if [ ! -d src ]; then
    echo "Erreur: Impossible de trouver les fichiers à compiler !"
    exit 4
fi

# Check that the number of arguments is correct
if [[ $# -ne 2 && ( "$1" == "histo" || "$1" == "leaks" ) ]]; then
    echo "$help_message"
    echo "Erreur: Vous n'avez pas entré le nombre correct d'arguments pour '$1'"
    exit 5
fi

if [ "$1" = "histo" ]; then
    case "$2" in ("max"|"src"|"real")
        ;;
        *)
            echo "$help_message"
            echo "Erreur: L'argument '$2' est invalide pour '$1' ! "
            exit 6
        ;;
    esac
fi

make

if [ ! -f bin/cwildwater ]; then
    echo "Erreur: La compilation a échoué !"
    exit 7
fi

chmod +x bin/cwildwater

rights=`ls -lh cwildwater.sh | cut -d " " -f 1`
if [ ${rights: -1} != "x" ]; then
    echo "Erreur: Le fichier binaire n'a pas pu être rendu exécutable !"
fi

./bin/cwildwater
make clean


if [ -f bin/cwildwater ]; then
    echo "Erreur: Le nettoyage a échoué !"
    exit 8
fi
