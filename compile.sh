#!/bin/bash

# Usage : ./script.sh chemin dossier serror|werror [options_execution...]

if [ "$3" = "serror" ]; then
    # Compilation avec seulement -Wall
    clear && gcc -Wall ./"$1"/"$2".c -o ./"$1"/output/"$2" && ./"$1"/output/"$2" "${@:4}"
elif [ "$3" = "werror" ]; then
    # Compilation avec -Wall et -Werror
    clear && gcc -Wall -Werror ./"$1"/"$2".c -o ./"$1"/output/"$2" && ./"$1"/output/"$2" "${@:4}"
else
    echo "Troisième argument non valide : utilisez 'serror' ou 'werror'."
fi
