#!/bin/bash
# Find recursively in a directory, all the files with the extension ".log" and sort their lines (replace the original file with the sorted content).
if [ $# -lt 1 ]; then
	echo "Invalid directory"
	exit 1
fi

D=$1
for FILE in `find $D -name *.log`; do
	C=`grep -E "" $FILE | sort`
	echo $C > $FILE
done
