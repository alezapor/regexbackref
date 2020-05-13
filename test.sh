#!/bin/bash
simpleOut="tests/out/$1.out" 
avdOut="tests/out/${1}2.out"
avd2Out="tests/out/${1}3.out"
ref="tests/$1.ref"
in="tests/$1.in"
lineCnt=$(cat "$in" | wc -l)
lineCnt=$((lineCnt/2))

touch "$avdOut"
> "$avdOut"
touch "$simpleOut"
> "$simpleOut"
touch "$avd2Out"
> "$avd2Out"

if [ $lineCnt -ne 0 ]
then
        for (( i=0; i<$lineCnt; i++ ))
	do
		line1=$(cat "$in"  | tail -n +$((2*i+1)) | head -n 1)
		line2=$(cat "$in"  | tail -n +$((2*i+2)) | head -n 1) 
		./regexmatcher 0 "$line1" "$line2" >> "$simpleOut"
		./regexmatcher 1 "$line1" "$line2" >> "$avdOut"
		./regexmatcher 2 "$line1" "$line2" >> "$avd2Out"
	done
	if diff "$simpleOut" "$ref";
	then	
		echo "All tests from $1 passed for simpleTM"
	else
		echo "There is an error for some test of $1 for simpleTM"	
	fi
	if diff "$avdOut" "$ref";
	then	
		echo "All tests from $1 passed for simpleMemory"
	else
		echo "There is an error for some test of $1 for simpleMemory"	
	fi
	if diff "$avd2Out" "$ref";
	then	
		echo "All tests from $1 passed for avdMemory"
	else
		echo "There is an error for some test of $1 for avdMemory"	
	fi
else
        echo "No tests in a test file"
fi

