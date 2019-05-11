#!/bin/bash

shopt -s expand_aliases
source ~/.bash_aliases

for f in *; do
    if [[ -d $f ]]; then
        if [ -f ./$f/gauss_on_times.txt ] && [ -f ./$f/gauss_off_times.txt  ]; then
            cd $f
            echo $f
            ministat gauss_*.txt -qc 80 | grep "confidence"  >> confidence80.txt
            cd ..
        fi
    fi
done
