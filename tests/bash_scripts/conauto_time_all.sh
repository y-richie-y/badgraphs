#!/bin/bash

for f in *; do
    if [[ -d $f ]]; then
        # $f is a directory
        cd $f
        echo $f $OPTION
        sbatch --partition=skylake-himem --time=3:0:0 ../conauto_time.sh
        cd ..
    fi
done
