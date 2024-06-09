#!/bin/bash
echo "Hello world! $1"  # digits for chmod 1:user 2:group 3:others
# $@ - the vector of arguments  
# $# - the nr of arguments
for A in $@; do
	echo "$A "
done
B=1
echo B # this will print the letter B
echo "$B " # this will print the value of the variable B
