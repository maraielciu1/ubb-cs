# sa se numere cate fisiere .txt contin toate cuvintele date ca parametri
#!/bin/bash

if [ $# -lt 1 ]; then
	echo "invalid args"
	exit 1
fi

D=$1
shift
if [ ! -d $D ]; then
	echo "$D is not a directory"
	exit 1
fi
count=0
for file in `find $D -name "*.txt"`; do
	ok=1
	for cuv in $@; do
		if ! grep -Eq "$cuv" "$file"; then
          	 	ok=0
            		break
        	fi
	done
	if [ $ok -eq 1 ]; then
		count=`expr $count + 1`
	fi
done
echo $count
 
