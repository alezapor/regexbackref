#!/bin/bash
grepOut="tests/out/grep$1.out" 
ref="tests/$1.ref"
in="tests/grep$1.in"
lineCnt=$(cat "$in" | wc -l)
lineCnt=$((lineCnt/2))

touch "$grepOut"
> "$grepOut"

if [ $lineCnt -ne 0 ]
then
        for (( i=0; i<$lineCnt; i++ ))
	do
		line1=$(cat "$in"  | tail -n +$((2*i+1)) | head -n 1)
		line2=$(cat "$in"  | tail -n +$((2*i+2)) | head -n 1) 
		echo "$line2" | grep -E -w "$line1" > /dev/null
		if [ $? -eq 0 ]
		then
			echo "yes" >> "$grepOut"
		else
			echo "no" >> "$grepOut"
		fi
	done
	if diff "$grepOut" "$ref";
	then	
		echo "All tests from $1 passed for grep"
	else
		echo "There is an error for some test of $1 for grep"	
	fi
else
        echo "No tests in a test file"
fi

