# pentru fiecare litera data ca parametru, sa se creeze un fisier cu toate cuvintele din f.txt care ibcep cu litera respectiva
#!/bin/bash

if [ ! -f $1 ]; then
	echo "Fisier invalid"
	exit 1
fi

FISIER=$1
shift
for LITERA in $@; do
	if echo $LITERA | grep -E -q "^[a-z]$"; then
		grep -E -i -o "\<$LITERA[a-z]*\>" $FISIER > "${LITERA}.txt"
	else
		echo "$LITERA nu este o litera mica"
	fi
done 
