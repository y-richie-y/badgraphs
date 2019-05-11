#!/bin/bash

if [[ "$#" !=  "2" ]]; then
    echo "Usage: [n] [name]"
    exit
fi

n=$1
FOLDER_NAME=$2

# make folder in case it does not exist
mkdir -p $FOLDER_NAME

if [[ $(( $n % 2 )) == 1 ]]; then
    echo "n must be even."
    exit
fi

./ns_base $n $FOLDER_NAME

cd $FOLDER_NAME
../mult base.dmc $(( 2*$n )) $(( 3*$n/2 ))
cd ..

# clean intermediate files
if [[ $* == *--clean* ]]; then
    cd $FOLDER_NAME
    rm base.dmc base.dre mult.dmc
    cd ..
fi

exit $EXIT_CODE
# call construction here
