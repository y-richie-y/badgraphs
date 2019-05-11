#!/bin/bash

for f in *; do
    if [[ -d $f ]]; then
        # $f is a directory
        #if [ $(cat ./$f/times.txt | grep "seconds" | wc -l) == 1 ]; then
        if [ ! -f ./$f/gauss_off_times.txt ] && [ -f ./$f/clauses.xor ]; then
            cd $f
            echo $f
            #sbatch --partition=skylake-himem --time=8:0:0 ../time_sat.sh
            ../time_sat.sh
            cd ..
        fi
    fi
done
