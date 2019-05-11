#!/bin/bash

if [[ "$#" -lt  "3" ]]; then
    echo "Usage: [m] [n] [name]"
    exit
fi

m=$1
n=$2
FOLDER_NAME=$3

mkdir -p $FOLDER_NAME

# clear log file before starting
if [[ -f ./${FOLDER_NAME}/filters.log ]]; then
    rm ./${FOLDER_NAME}/filters.log
fi

# use module to load parallel tool if on HPC
command -v module >/dev/null 2>&1 || module load parallel

run_filters () {
    ./d_base $m $n $FOLDER_NAME
    parallel --no-notice -j 0 --halt 2 :::  "./filters/group_size.sh ${FOLDER_NAME}/base.dre" "./filters/rank ${FOLDER_NAME}/triples.tri" 2>/dev/null >> ${FOLDER_NAME}/filters.log 
    EXIT_CODE=$?    
}

i=0
run_filters

if ! [[ $* == *--no-filters* ]]; then
    while [[ "$EXIT_CODE" != "0" ]]; do
        i=$(($i+1))
        run_filters
    done
fi

echo "All filters passed - final graph is asymmetric!" | tee -a ${FOLDER_NAME}/filters.log
echo "$i restarts required." | tee -a ${FOLDER_NAME}/filters.log

# call construction here
cd $FOLDER_NAME
../mult base.dmc $m $n 
cd ..

# clean intermediate files
if [[ $* == *--clean* ]]; then
    cd ${FOLDER_NAME}
    rm base.dmc base.dre mult.dmc filters.log triples.tri clauses.cnf
    cd ..
fi

exit $EXIT_CODE
