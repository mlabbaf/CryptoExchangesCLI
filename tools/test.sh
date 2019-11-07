while [[ true ]]; 
do 
	echo "83" | ./Run.sh | tee -a log
	echo "31 1" | ./Run.sh | grep symbol | tee -a log
	echo "22" | ./Run.sh | grep symbol
	date
	sleep 30
done

