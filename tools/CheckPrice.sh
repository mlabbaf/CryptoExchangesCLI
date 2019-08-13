#!/bin/bash

root_dir=$(cd $(dirname "$0"); pwd)
echo $root_dir

cd $root_dir/..

while [ true ]
do
	# echo "80" | ./Run.sh > log
	str=`cat log | tail -n 13 | head -n 4 | cut -f 3 | sed 's/\(.*\) (.*/\1/g'`
	p1=`echo $str | cut -d " " -f 1`
	p2=`echo $str | cut -d " " -f 2`
	p3=`echo $str | cut -d " " -f 3`
	p4=`echo $str | cut -d " " -f 4`
	
	Threshold="133500"
	# echo "p1: $p1"
	if [[ $p1 < $Threshold && $p1 != "0" ]]
	then 
		echo $p1 " for usdtirr"
	fi
	
	# echo "p2: $p2"
	if [[ $p2 < $Threshold && $p2 != "0" ]]
        then
                echo $p2 " for btcirr"
        fi
	
	# echo "p3: $p3"
	if [[ $p3 < $Threshold && $p3 != "0" ]]
        then
                echo $p3 " for ethirr"
        fi
	
	# echo "p4: $p4"
	if [[ $p3 < $Threshold && $p4 != "0" ]]
        then
        	echo $p3 " for xrpirr"
        fi	

	sleep 600
done

