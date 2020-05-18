#!/bin/bash
TIMEFORMAT=%R
timeSimple=0
timeAvd=0
timeAvd2=0
timeGrep=0
timePerl=0
testFile="tests/$1.in"
testGrep="tests/grep$1.in"
testPerl="tests/perl$1.in"
lineCnt=$(cat "$testFile" | wc -l)
lineCnt=$((lineCnt/2))
if [ $lineCnt -ne 0 ]
then
        for (( i=0; i<$lineCnt; i++ ))
	do
		line1=$(cat "$testFile" | tail -n +$((2*i+1)) | head -n 1)
		line2=$(cat "$testFile" | tail -n +$((2*i+2)) | head -n 1) 
		line1G=$(cat "$testGrep" | tail -n +$((2*i+1)) | head -n 1)
		line2G=$(cat "$testGrep" | tail -n +$((2*i+2)) | head -n 1) 
		line1P=$(cat "$testPerl"  | tail -n +$((2*i+1)) | head -n 1)
		line2P=$(cat "$testPerl"  | tail -n +$((2*i+2)) | head -n 1) 
		#uncomment for testing simpleTM
		#testTime=$( { time { ./regexmatcher 0 "$line1" "$line2"; } } 2>&1 1>\dev\null ) 
		#echo "Test $i passed simpleTM, real time $testTime"
		#timeSimple=$(echo "$timeSimple+$testTime" | bc)

		testTime=$( { time { ./regexmatcher 1 "$line1" "$line2"; } } 2>&1 1>\dev\null ) 
		echo "Test $i passed simpleMemory, real time $testTime"
		timeAvd=$(echo "$timeAvd+$testTime" | bc)

		testTime=$( { time { ./regexmatcher 2 "$line1" "$line2"; } } 2>&1 1>\dev\null ) 
		echo "Test $i passed avdMemory, real time $testTime"
		timeAvd2=$(echo "$timeAvd2+$testTime" | bc)

		testTime=$( { time { echo "$line2G" | grep -E -w "$line1G" > /dev/null; } } 2>&1 1>\dev\null ) 
		echo "Test $i passed grep, real time $testTime"
		timeGrep=$(echo "$timeGrep+$testTime" | bc)

		testTime=$( { time { perl -e "if ('$line2P' =~ /^$line1P$/) {exit}"; } } 2>&1 1>\dev\null ) 
		echo "Test $i passed perl, real time $testTime"
		timePerl=$(echo "$timePerl+$testTime" | bc)
	done
	echo "All tests from $1 passed simpleTM, real time: $timeSimple"
	echo "All tests from $1 passed simpleMemory, real time: $timeAvd"
	echo "All tests from $1 passed avdMemory, real time: $timeAvd2"
	echo "All tests from $1 passed grep, real time: $timeGrep"
	echo "All tests from $1 passed perl, real time: $timePerl"
else
        echo "No tests in a test file"
fi

