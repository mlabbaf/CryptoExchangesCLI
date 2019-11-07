#!/bin/sh


SLEEP_TIME=300


while [ true ]
do
	date
	echo -n "\n"

	sleep 5

	####################################################################################
	echo "Binance prices:"
	temp=`echo "21 3" | ./Run.sh | grep --color "\"BTC"`
	echo -n "\t$temp\n"
	temp=`echo "21 4" | ./Run.sh | grep --color "\"ETH"`
	echo -n "\t$temp\n"
	temp=`echo "21 5" | ./Run.sh | grep --color "\"NEO"`
	echo -n "\t$temp\n"
	temp=`echo "21 6" | ./Run.sh | grep --color "\"ONT"`
	echo -n "\t$temp\n"
	temp=`echo "21 7" | ./Run.sh | grep --color "\"ADA"`
	echo -n "\t$temp\n"
	temp=`echo "21 8" | ./Run.sh | grep --color "\"TRX"`
	echo -n "\t$temp\n"
	temp=`echo "21 1O" | ./Run.sh | grep --color "\"XRPUSDT"`
	echo -n "\t$temp\n"
	temp=`echo "21 12" | ./Run.sh | grep --color "\"BTT"`
	echo -n "\t$temp\n"
	temp=`echo "21 13" | ./Run.sh | grep --color "\"BNB"`
	echo -n "\t$temp\n"
	temp=`echo "21 14" | ./Run.sh | grep --color "\"LTC"`
	echo -n "\t$temp\n\n"


	####################################################################################
	echo "Open orders:"
	temp=`echo "31 4" | ./Run.sh | grep --color "\"ETH"`
	echo -n "$temp\n"
	temp=`echo "31 5" | ./Run.sh | grep --color "\"NEO"`
	echo -n "$temp\n"
	temp=`echo "31 6" | ./Run.sh | grep --color "\"ONT"`
	echo -n "$temp\n"
	temp=`echo "31 7" | ./Run.sh | grep --color "\"ADA"`
	echo -n "$temp\n"
	temp=`echo "31 8" | ./Run.sh | grep --color "\"TRX"`
	echo -n "$temp\n"
	temp=`echo "31 10" | ./Run.sh | grep --color "\"XRP"`
	echo -n "$temp\n"
	temp=`echo "31 12" | ./Run.sh | grep --color "\"BTT"`
	echo -n "$temp\n"
	temp=`echo "31 13" | ./Run.sh | grep --color "\"BNB"`
	echo -n "$temp\n"
	temp=`echo "31 14" | ./Run.sh | grep --color "\"LTC"`
	echo -n "$temp\n\n"

	####################################################################################
	echo "Trades:"
	temp=`echo "33 4 1" | ./Run.sh | grep --color "\"ETH"`
	echo -n "$temp\n"
	temp=`echo "33 5 1" | ./Run.sh | grep --color "\"NEO"`
	echo -n "$temp\n"
	temp=`echo "33 6 1" | ./Run.sh | grep --color "\"ONT"`
	echo -n "$temp\n"
	temp=`echo "33 7 1" | ./Run.sh | grep --color "\"ADA"`
	echo -n "$temp\n"
	temp=`echo "33 8 1" | ./Run.sh | grep --color "\"TRX"`
	echo -n "$temp\n"
	temp=`echo "33 10 1" | ./Run.sh | grep --color "\"XRP"`
	echo -n "$temp\n"
	temp=`echo "33 12 1" | ./Run.sh | grep --color "\"BTT"`
	echo -n "$temp\n"
	temp=`echo "33 13 1" | ./Run.sh | grep --color "\"BNB"`
	echo -n "$temp\n"
	temp=`echo "33 14 1" | ./Run.sh | grep --color "\"LTC"`
	echo -n "$temp\n\n"

	####################################################################################
	echo "Dollar:"
	temp=`echo "81" | ./Run.sh | grep --color -e "Minimum Buy option" -e "Maximum Sell option"`
	echo -n "$temp\n\n"

	####################################################################################
	# echo "Sculping bot:"
	# temp=`echo "83" | ./Run.sh | grep "Price of"`
	# echo -n "$temp\n\n"

	####################################################################################
	# echo "Hodling bot:"
	# temp=`echo "84" | ./Run.sh | grep --color -e "Symbol" -e "Price of"`
	# echo -n "$temp\n\n"

	####################################################################################
	echo "Whole Balance:"
	temp=`echo "23" | ./Run.sh | grep -e "wholeBalance" -e "price"`
	echo -n "$temp\n\n"
	date >> wholeBalance
	echo $temp >> wholeBalance

	####################################################################################
	echo -n "sleep for $SLEEP_TIME\n"
	sleep $SLEEP_TIME
done
