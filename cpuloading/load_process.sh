#!/bin/bash
#cpu load process shell
# 0319 1638
# actually work!!
#variables
IP=
time=
limit=100
# functions
function info() {
	echo "load_cpu [[-ip] [-t] [-i]]"
}

# main
# arguments
while [ "$1" != "" ]; do
	case "$1" in
		-ip	)	shift
				IP=$1
				;;
		-t	)	shift
				if [ "$1" -gt 100 ];	then
					
					echo "duration should be greater than 0 !"
					exit 1
				else
					time=$1
				fi
				;;
		-i	)	shift
				if [ "$1" -gt 100 -o "$1" -lt 0 ];	then
				# -o: condition or
					echo "limit range between 0 to 100 !"
					exit 1
				else
					limit=$1
				;;
		*	)	info;
				exit 1
				;;
	esac
	shift
done

echo "load cpu limit $limit % over $IP for $time secs."
#echo "ssh root@$IP 'cpulimit -l $limit ./loop'"
ssh root@$IP "./loop $limit $time &"
#cpulimit -l $limit ./loop & sleep $time; pkill loop
# &: exec as background process
exit 0
