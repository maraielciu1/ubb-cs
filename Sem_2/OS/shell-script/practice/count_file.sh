# count the nr of occurences of a file in a dir and all its subdirs
#!/bin/bash

if [ ! $# -eq 2 ]; then
	echo "invalid number of parameters"
	exit 1
fi

if [ ! -d $1 ]; then
	echo "invalid directory"
	exit 1
fi

if [ ! -f $2 ]; then
	echo "invalid file"
	exit 1
fi

D=$1
F=$2
COUNT=0
for FILE in `find $D -type f`; do
	if [ "$FILE"=="$F" ]; then
		COUNT=`expr $COUNT + 1`
	fi
done
echo $COUNT		
