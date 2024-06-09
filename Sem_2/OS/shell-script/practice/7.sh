#Consider a file containing a username on each line. Generate a comma-separated string with email addresses of the users that exist. The email address will be obtained by appending "@scs.ubbcluj.ro" at the end of each username. Make sure the generated string does NOT end in a comma.
#!/bin/bash

if [ ! $# -eq 1 ]; then
	exit 1
fi
FILE=$1
result=""
for LINE in `cat $FILE`; do
	 result="$result,$LINE@scs.ubbcluj.ro"
done
echo $result | sed -E "s/^,//"

