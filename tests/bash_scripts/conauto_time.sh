#!/bin/bash

OPTION=$1

shopt -s expand_aliases
source ~/.bash_aliases

conauto -aut -d  mult.dmc > conauto_time.txt

echo ${PWD##*/} > conauto_summary.txt
cat conauto_time.txt | grep "seconds" >> conauto_summary.txt
