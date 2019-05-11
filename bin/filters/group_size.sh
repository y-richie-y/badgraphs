#!/bin/bash

shopt -s expand_aliases
source ~/.bash_aliases

GRPSIZE_STR=$(echo "At < $1 x" | dreadnaut | egrep "grpsize=[0-9]*" -o)

echo $GRPSIZE_STR

# trim away "grpsize="
# return non-zero exit code if |Aut(G)| > 1
if [ "${GRPSIZE_STR:8}" == "1" ]; then
    exit 0
else 
    exit 1
fi
