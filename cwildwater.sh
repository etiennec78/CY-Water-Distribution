start=$(date +%s%N)

help_message="""
C-WildWater: Un script pour effectuer un histogramme
des usines de traitement d'eau à partir d'un fichier .dat.

Utilisation: ./cwildwater.sh <db_file> [arg] [subarg]

Arguments:
    histo [subarg]: Créé un histogramme à partir des sous-arguments suivants:
        max: Volume maximal de traitement de l'usine
        src: Volume total capté par les sources
        real: Volume total réellement traité

    leaks <id_usine>:
        Effectue la mesure des pertes d'une usine sur l'ensemble de son réseau aval.
        L'identifiant d'usine doit être complet et respecter la casse.

    help:
        Afficher ce message
"""

# Print the help message if requested
for arg in $@; do
    case "$arg" in ("--help"|"help"|"-h")
            echo "$help_message"
            exit 0
        ;;
    esac
done

# Check that enough args were given
if [ $# -ne 3 ]; then
    echo "$help_message"
    echo "Erreur: Vous n'avez pas entré le nombre correct d'arguments !"
    exit 1
fi

# Check that the database file exists
if [ ! -f "$1" ]; then
    echo "Erreur: Le fichier de données '$1' n'existe pas !"
    exit 2
fi

# Check that the second argument is valid
case "$2" in ("histo"|"leaks")
    ;;
    *)
        echo "$help_message"
        echo "Erreur: Votre argument '$2' n'est pas valide ! "
        exit 3
    ;;
esac

if [ "$2" = "histo" ]; then
    case "$3" in ("max"|"src"|"real"|"all")
        ;;
        *)
            echo "$help_message"
            echo "Erreur: L'argument '$3' est invalide pour '$2' ! "
            exit 4
        ;;
    esac
fi

# Check that the factory exists in the file
if [[ "$2" = "leaks" ]] && ! grep -q "$3" "$1"; then
    echo "Erreur: L'id d'usine '$3' n'a pas été trouvé dans le fichier $1";
    exit 5
fi

# Check that the makefile is present
if [ ! -f Makefile ]; then
    echo "Erreur: Impossible de trouver le makefile !"
    exit 6
fi

# Check that the src dir is present
if [ ! -d src ]; then
    echo "Erreur: Impossible de trouver les fichiers à compiler !"
    exit 7
fi

# Check for the presence of the binary
if [ ! -f bin/cwildwater ]; then

    make

    if [ ! -f bin/cwildwater ]; then
        echo "Erreur: La compilation a échoué !"
        exit 8
    fi
fi

chmod +x bin/cwildwater

if [ ! -x cwildwater.sh ]; then
    echo "Erreur: Le fichier binaire n'a pas pu être rendu exécutable !"
    exit 9
fi

./bin/cwildwater "$@"

# Check that the execution was successfull
if [ $? -ne 0 ]; then
    echo "Erreur: Un problème est survenu en exécutant le binaire !"
fi

# Get the location of the data file
case "$3" in
    "max")
    file_name="data/vol_max"
    ;;
    "src")
    file_name="data/vol_captation"
    ;;
    "real")
    file_name="data/vol_traitement"
    ;;
    "all")
    file_name="data/histo_all"
    ;;
    *)
    file_name=""
esac

# If the binary should create an output file
if [[ "$file_name" != "" ]]; then

    # Check that the output file exists
    if [ ! -f "$file_name.dat" ]; then
        echo "Erreur: Le fichier de données n'a pas été créé !"
        exit 11
    fi

    # Check that Python is installed
    if [[ ! $(python --version 2>/dev/null) ]]; then
        echo "Erreur: Python n'est pas installé !"
        exit 10
    fi

    # Check that matplotlib is installed
    if ! $(python -c "import matplotlib" 2>/dev/null); then
        echo "Erreur: La bibliothèque Python matplotlib n'est pas installée : https://matplotlib.org/stable/install/index.html !"
        exit 14
    fi

    python src/Plotting/plotting.py "$file_name.dat"

    # Check that the execution was successfull
    if [ $? -ne 0 ]; then
        echo "Erreur: Un problème est survenu en essayant de créer l'histogramme !"
        exit 12
    fi

    for suffix in "low" "high"; do
        # Check that the graph picture was created
        png_file="${file_name}_${suffix}.png"
        if [ ! -f "$png_file" ]; then
            echo "Erreur: Un des histogrammes n'a pas été généré !"
            exit 13
        fi

        echo "Info: L'histogramme $suffix a été stocké à cet emplacement: `pwd`/$png_file"
    done
fi

end=$(date +%s%N)
exec_time=$(( (end - start) / 1000000 ))

echo "Info: Fin de l'exécution en $((exec_time))ms"
