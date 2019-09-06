#ifndef DEFINITION_H
#define DEFINITION_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <exception>
#include <fstream>
#include <algorithm>
#include <sys/time.h>

#include <json/json.h>

#include "Log.h"


using namespace std;


enum _ExchangeExist {
	BINANCE	 	= 0x01,
	RAMZINEX 	= 0x02,
	COINEX 		= 0x04,
	HITBTC 		= 0x08,
	KUCOIN 		= 0x10,
	ALL 		= 0X80
};

enum _GetBalancesModes {
	ONLY_BANK_MODE = 0,
	ONLY_EXCHANGE_MODE,
	BANK_AND_EXCHANGE_MODE
};

enum _BotHodlingTradingModes {
	ONLY_HODLING_MODE = 0,
	ONLY_TRADING_MODE,
	HODLING_AND_TRADING_MODE
};

struct StructBalanceInUSDT {
	double balance;
	double balanceFree;
	double balanceLocked;
	double price;
	double asset;
};

struct StructKucoinHttpFields {
	string apiKey;
	string signature;
	string timestamp;
	string passphrase;
};

struct SymbolPriceSrtuct {
	string symbol;
	double price;
};

struct strcutExchangesPrices{
	double binancePrice;
	double ramzinexPrice;
	double coinexPrice;
	double hitBTCPrice;
	double kucoinPrice;
};

#endif