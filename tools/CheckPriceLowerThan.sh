#!/bin/bash

root_dir=$(cd $(dirname "$0"); pwd)
echo $root_dir

cd $root_dir/..

while [ true ]
do
	str=`echo "20 4" | ./Run.sh | grep "symbol"`
	price=`echo $str | sed 's/^.*price\":\"\(.*\)\",.*$/\1/g'`
	
	if [[ $price < "150" ]]
	then
		date
		echo "Price is low enough. price: $price"
	# else
	#	date
	#	echo "price: $price"
	fi

	sleep 600
done

