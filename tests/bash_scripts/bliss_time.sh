#!/bin/bash

OPTION=$1

shopt -s expand_aliases
source ~/.bash_aliases

bliss -sh=$OPTION mult.dmc > bliss_times_${OPTION}.txt

#echo "An < mult.dre x At < mult.dre x" | dreadnaut > times.txt
#echo "An < mult.dre x" | dreadnaut > times_just_nauty.txt
