#!/bin/bash

for f in *; do
    if [[ -d $f ]] && [[ $f != "size" ]]; then
        # $f is a directory
        #if [ $(cat ./$f/times.txt | grep "seconds" | wc -l) == 1 ]; then
        if [ ! -f ./$f/times.txt ]; then
            cd $f
            echo $f
            sbatch --partition=skylake-himem --time=8:0:0 ../time_nauty.sh
            sbatch --partition=skylake-himem --time=8:0:0 ../time_traces.sh
            cd ..
        fi
    fi
done
