#!/bin/bash
#Find recursively in a directory all ".c" files having more than 500 lines. Stop after finding 2 such files.
CNT=0
if [ $# -lt 1 ]; then
	echo "Invalid directory"
	exit 1
fi

D=$1
for FILE in `find "$D" -name *.c`; do
	if [ $CNT -lt 2 ]; then
		N=`grep -E "" $FILE | wc -l`
		if [ $N -gt 500 ]; then
			echo $FILE
			CNT=`expr $CNT + 1`
		fi
	else
		break
	fi
done 
