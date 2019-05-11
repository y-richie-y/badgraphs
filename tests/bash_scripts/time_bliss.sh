#!/bin/bash

for f in *; do
    if [[ -d $f ]]; then
        # $f is a directory
        #if [ $(cat ./$f/times.txt | grep "seconds" | wc -l) == 1 ]; then
        if [ ! -f ./$f/times.txt ]; then
            cd $f
            echo $f
            sbatch --partition=skylake-himem --time=8:0:0 ../time.sh
            cd ..
        fi
    fi
done
