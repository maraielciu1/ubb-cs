#!/bin/bash

if [ $(expr $# % 2) -eq 1 ]; then
    echo "Invalid number of arguments"
    exit 1
fi

echo "" > result.txt

while [ $# -gt 0 ]; do
    dir=$1
    file=$2

    if [ ! -d "$dir" ]; then
        echo "$dir is not a valid directory"
    elif [ ! -f "${dir}/${file}" ]; then
        echo "$file is not a valid file in $dir"
    else
        # Count occurrences of exact filename within directory structure
        count=$(find "$dir" -type f -name "$file" | wc -l)
        echo "$dir $file $count" >> result.txt
    fi
    
    # Move to the next pair of directory/file arguments
    shift 2
done

# Sort the results by the count, numerically, and display
sort -n -k3 result.txt > sorted_result.txt
cat sorted_result.txt

