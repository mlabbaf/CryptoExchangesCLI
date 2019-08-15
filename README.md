# CryptoExchangesCLI
A CLI (Command Line Interface) is developed to access different cryptocurrency exchanges.
This source codes are built and tested in linux.


In order to Build the source code, only run the Build.sh script. After this, bin directory is created and resultant binary is placed there.
After building the binary, you can use the Run.sh script to only run the binary without building the new binary file.

Running the binary requires API and secret key for all exchanges. So, you should place them in config folder when you want to Run the program. Sample keys are placed in config directory.

In order to test the source code, you can only copy config/xyzKeys.txt.sample to config/xyzKeys.txt file, for example:
cp config/BinanceKeys.txt.sample config/BinanceKeys.txt

following exchanges are supported:

	Binance
	Coinex
	HitBTC
	Kucoin

Warning: The user is responsible for the results of using the source code.
