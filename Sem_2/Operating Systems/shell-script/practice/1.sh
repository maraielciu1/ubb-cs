# Display a report showing the full name of all the users currently connected, and the number of processes belonging to each of them.
#!/bin/bash
NAMES=`awk {'print $1'} who.fake`
for name in $NAMES; do
	N=`grep -E "$name" ps.fake | wc -l`
	echo "$name $N"
done
