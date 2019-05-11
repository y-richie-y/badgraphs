#!/bin/bash

shopt -s expand_aliases
source ~/.bash_aliases

echo "Nauty" > nauty_times.txt
echo "An < mult.dre x" | dreadnaut >> nauty_times.txt

echo "Traces" > traces_times.txt
echo "At < mult.dre x" | dreadnaut >> traces_times.txt
