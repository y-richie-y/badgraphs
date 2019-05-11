#!/bin/bash

for f in *; do
    if [[ -d $f ]]; then
        # $f is a directory
        cd $f
        if [[ -f times.txt ]]; then
            echo $f >> ../summary_depth.txt
            awk '/maxlev/ {a=$NF} END{print a} /seconds/ {b=$((NF-1))} END{print b " seconds"}' times.txt >> ../summary_depth.txt
        fi
        cd ..
    fi
done
