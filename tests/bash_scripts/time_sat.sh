#!/bin/bash

shopt -s expand_aliases
source ~/.bash_aliases

ITER=20

for ((i=0;i<ITER;i++)); do
    GAUSS_OFF=`(time (cryptominisat --verb 0 clauses.xor) 3>&2 2>&1 1>&3) 2>/dev/null | grep "user"`
    GAUSS_ON=`(time (cryptominisat_gauss --verb 0 clauses.xor) 3>&2 2>&1 1>&3) 2>/dev/null | grep "user"`
    echo ${GAUSS_ON:7:10} | sed -e "s/s//" >> gauss_on_times.txt
    echo ${GAUSS_OFF:7:10} | sed -e "s/s//" >> gauss_off_times.txt
done
#cat gauss_off_times.txt | sed -e "s/s//"
#cat gauss_on_times.txt | sed -e "s/s//"
