#!/bin/bash
#Find recursively in a given directory all the symbolic links, and report those that point to files/directories that no longer exist. Use operator -L to test if a path is a symbolic link, and operator -e #to test if it exists (will return false if the target to which the link points does not exist)
if [ $# -lt 1 ]; then
	echo "Invalid args"
	exit 1
fi

D=$1
if [ ! -d $D ]; then
	echo "$D not a dir"
	exit 1
fi

for FILE in `find $D`; do
	if [ -L $FILE ]; then
		echo "$FILE is a symbolic link"
	fi
done
