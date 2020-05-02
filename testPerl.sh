#!/bin/bash
perlOut="tests/out/perl$1.out" 
ref="tests/$1.ref"
in="tests/perl$1.in"
lineCnt=$(cat "$in" | wc -l)
lineCnt=$((lineCnt/2))

touch "$perlOut"
> "$perlOut"

if [ $lineCnt -ne 0 ]
then
        for (( i=0; i<$lineCnt; i++ ))
	do
		line1=$(cat "$in"  | tail -n +$((2*i+1)) | head -n 1)
		line2=$(cat "$in"  | tail -n +$((2*i+2)) | head -n 1) 
		perl -e "if ('$line2' =~ /^$line1$/) {exit 0} else {exit 1}"
		if [ $? -eq 0 ]
		then
			echo "yes" >> "$perlOut"
		else
			echo "no" >> "$perlOut"
		fi
	done
	if diff "$perlOut" "$ref";
	then	
		echo "All tests from $1 passed for perl"
	else
		echo "There is an error for some test of $1 for perl"	
	fi
else
        echo "No tests in a test file"
fi

