#!/bin/bash

declare -a OPTIONS=("fm" "flm" "fsm")

for f in *; do
    if [[ -d $f ]] && [[ ! -s $f/nauty_times.txt ]];  then
        for OPTION in ${OPTIONS[@]}; do
            cd $f
            echo $f $OPTION
            sbatch --partition=skylake-himem --time=8:0:0 ../bliss_time.sh $OPTION
            cd ..
        done
        #fi
    fi
done
