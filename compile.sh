#!/bin/bash

if [ "$3" = "serror" ]; then
    clear && gcc -Wall ./"$1"/"$2".c -o ./"$1"/output/ex1 && ./"$1"/output/ex1
else
    clear && gcc -Wall -Werror ./"$1"/"$2".c -o ./"$1"/output/ex1 && ./"$1"/output/ex1
fi