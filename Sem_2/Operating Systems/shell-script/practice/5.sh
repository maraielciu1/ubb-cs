#Write a script that receives dangerous program names as command line arguments. The script will monitor all the processes in the system, and whenever a program known to be dangerous is run, the script wi#ll kill it and display a message.
#!/bin/bash
while true; do
	for prog in $@; do
		if [ `grep -E -q "^$prog" ps.fake` ]; then
			echo "killed $prog"
		fi
		# kill $(ps -e | grep -E $name | awk '{print $1}')
	done
done
