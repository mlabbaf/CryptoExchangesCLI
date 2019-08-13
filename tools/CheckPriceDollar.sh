#!/bin/bash

root_dir=$(cd $(dirname "$0"); pwd)
echo $root_dir

cd $root_dir/..

while [ true ]
do
	str=`echo "81" | ./Run.sh | grep "Minimum Buy"`
	price_dollar=`echo $str | sed 's/^Minimum Buy option: \(.*\)\$/\1/g'`
	echo $price_dollar

	str=`echo "21 3" | ./Run.sh`
	price_BTC=`echo $str | sed 's/^.*price\":\"\(.*\)\",\"symbol.*$/\1/g'`
	echo $price_BTC

	# if [[ $price > "160" ]]
	#then
	#	date
	#	echo "Price is high enough. price: $price"
	#else
	#	date
	#	echo "price: $price"
	# fi

	sleep 600
done

