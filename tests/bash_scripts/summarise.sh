#!/bin/bash

for f in *; do
    if [[ -d $f ]]; then
        # $f is a directory
        cd $f
        if [[ -f nauty_times.txt ]] && [[ -f traces_times.txt ]]; then
            echo $f
            echo $f > summary.txt
            cat nauty_times.txt | grep "seconds" >> summary.txt
            cat traces_times.txt | grep "seconds" >> summary.txt
        elif [[ -f times.txt ]]; then 
            echo $f
            echo $f > summary.txt
            cat times.txt | grep "seconds" >> summary.txt
            # covert all .tri files into xor files
            #cat triples.tri | python ../make_xor.py > clauses.xor
        fi
        cd ..
    fi
done
