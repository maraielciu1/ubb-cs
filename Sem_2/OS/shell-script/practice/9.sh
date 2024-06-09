#Write a script that finds in a given directory hierarchy, all duplicate files and displays their paths. Hint: use checksums to detect whether two files are identical.
#!/bin/bash

if [ ! $# -eq 1 ]; then
	echo "Invalid arg"
	exit 1
fi

if [ ! -d $1 ]; then
    echo "Directory does not exist"
    exit 1
fi

D=$1

for f in `find $D -type f -exec md5sum {} \; | awk '{print $1}' | sort | uniq`; do
    echo $f
    for g in `find $D -type f -exec md5sum {} \; | grep -E $f | awk '{print $2}'`; do
        echo $g
    done
done
