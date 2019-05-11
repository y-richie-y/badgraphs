#!/bin/bash

declare -a OPTIONS=("f" "fs" "fl" "fm" "flm" "fsm")

for OPTION in ${OPTIONS[@]}; do
    echo $OPTION
    for f in *; do
        if [[ -d $f ]]; then
            # $f is a directory
            cd $f
            if [[ -f bliss_times_${OPTION}.txt ]]; then
                echo $f
                echo $f > bliss_summary_${OPTION}.txt
                cat bliss_times_${OPTION}.txt | grep "seconds" >> bliss_summary_${OPTION}.txt
            fi
            cd ..
        fi
    done

    # combine into 1 file
    cat */bliss_summary_${OPTION}.txt > summary_${OPTION}.txt
done
