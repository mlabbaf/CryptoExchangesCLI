#include "BotMethod.h"
#include "Menu.h"
#include "Binance.h"
#include "Ramzinex.h"
#include "Coinex.h"
#include "HitBTC.h"
#include "Kucoin.h"
#include "Log.h"
#include "Utility.h"
#include "Definitions.h"

using namespace std;

static Binance *binance;
static Coinex *coinex;
static HitBTC *hitbtc;
static Kucoin *kucoin;


void _findMinAndMaxFromStrcutExchangesPrices(strcutExchangesPrices &priceExchanges, double &min, double &max) {
	min = 10000000;
	max = 0;

	if (priceExchanges.binancePrice && min > priceExchanges.binancePrice)
		min = priceExchanges.binancePrice;
	if (priceExchanges.coinexPrice && min > priceExchanges.coinexPrice)
		min = priceExchanges.coinexPrice;
	if (priceExchanges.hitBTCPrice && min > priceExchanges.hitBTCPrice)
		min = priceExchanges.hitBTCPrice;
	if (priceExchanges.kucoinPrice && min > priceExchanges.kucoinPrice)
		min = priceExchanges.kucoinPrice;

	if (priceExchanges.binancePrice && max < priceExchanges.binancePrice)
		max = priceExchanges.binancePrice;
	if (priceExchanges.coinexPrice && max < priceExchanges.coinexPrice)
		max = priceExchanges.coinexPrice;
	if (priceExchanges.hitBTCPrice && max < priceExchanges.hitBTCPrice)
		max = priceExchanges.hitBTCPrice;
	if (priceExchanges.kucoinPrice && max < priceExchanges.kucoinPrice)
		max = priceExchanges.kucoinPrice;
} 

void sortMapBasedOnPriceCurrent(map <string, prices> &items, vector <pair<string, prices>> &items_sorted) {
	// Declaring the type of Predicate that accepts 2 pairs and return a bool
	typedef function <bool(pair<string, prices>, pair<string, prices>)> Comparator;
 
	// Defining a lambda function to compare two pairs. It will compare two pairs using second field
	Comparator compFunctorCurrent =
		[](pair<string, prices> elem1 ,pair<string, prices> elem2)
		{
			return elem1.second.current > elem2.second.current;
		};

	// Declaring a set that will store the pairs using above comparision logic
	set<pair<string, prices>, Comparator> setOfWords(
		items.begin(), items.end(), compFunctorCurrent);
 
	// Iterate over a set using range base for loop
	for (pair<string, prices> element : setOfWords) {
		items_sorted.push_back(element);
		// items[element.first] = element.second;
		// cout << element.first << " :: " << element.second.buy << endl;
	}
}

void sortMapBasedOnPriceDiff(map <string, prices> &items, vector <pair<string, prices>> &items_sorted) {
	// Declaring the type of Predicate that accepts 2 pairs and return a bool
	typedef function <bool(pair<string, prices>, pair<string, prices>)> Comparator;
 
	// Defining a lambda function to compare two pairs. It will compare two pairs using second field
	Comparator compFunctorDiff =
		[](pair<string, prices> elem1 ,pair<string, prices> elem2)
		{
			return elem1.second.diff > elem2.second.diff;
		};

	// Declaring a set that will store the pairs using above comparision logic
	set<pair<string, prices>, Comparator> setOfWords(
		items.begin(), items.end(), compFunctorDiff);
 
	// Iterate over a set using range base for loop
	for (pair<string, prices> element : setOfWords) {
		items_sorted.push_back(element);
		// items[element.first] = element.second;
		// cout << element.first << " :: " << element.second.buy << endl;
	}
}

void sortMapBasedOnAsset(map <string, StructBalanceInUSDT> &items, vector <pair<string, StructBalanceInUSDT>> &items_sorted) {
	// Declaring the type of Predicate that accepts 2 pairs and return a bool
	typedef function <bool(pair<string, StructBalanceInUSDT>, pair<string, StructBalanceInUSDT>)> Comparator;
 
	// Defining a lambda function to compare two pairs. It will compare two pairs using second field
	Comparator compFunctorCurrent =
		[](pair<string, StructBalanceInUSDT> elem1 ,pair<string, StructBalanceInUSDT> elem2)
		{
			return elem1.second.asset > elem2.second.asset;
		};

	// Declaring a set that will store the pairs using above comparision logic
	set<pair<string, StructBalanceInUSDT>, Comparator> setOfWords(
		items.begin(), items.end(), compFunctorCurrent);
 
	// Iterate over a set using range base for loop
	for (pair<string, StructBalanceInUSDT> element : setOfWords) {
		items_sorted.push_back(element);
		// items[element.first] = element.second;
		// cout << element.first << " :: " << element.second.buy << endl;
	}
}

void BotMethod::CheckArbitrage() {
	// bool arbitrageExist = false;

	// Json::Value jsonRamzinex, jsonBinance;
	// Json::FastWriter fastWriter;

	// Json::Value::const_iterator json_iter;

	// string str = "All";
	// jsonBinance = binance->GetPrices(str);
	// jsonRamzinex = Ramzinex::GetPrices(str);

	// map <string, StructDifferentExchangePrices> mapPrices;
	// map <string, StructDifferentExchangePrices>::iterator map_iter;

	// mapPrices["btcusdt"];
	// mapPrices["ethusdt"];
	// mapPrices["xrpusdt"];

	// for (json_iter = jsonBinance.begin(); json_iter != jsonBinance.end(); json_iter++) {
	// 	if ((*json_iter)["symbol"] == "XRPUSDT")
	// 		mapPrices["xrpusdt"].BinancePrice = Utility::JsonToDouble((*json_iter)["price"]);
	// 	if ((*json_iter)["symbol"] == "BTCUSDT") 
	// 		mapPrices["btcusdt"].BinancePrice = Utility::JsonToDouble((*json_iter)["price"]);
	// 	if ((*json_iter)["symbol"] == "ETHUSDT") 
	// 		mapPrices["ethusdt"].BinancePrice = Utility::JsonToDouble((*json_iter)["price"]);
	// }

	// for (map_iter = mapPrices.begin(); map_iter != mapPrices.end(); map_iter++) {
	// 	map_iter->second.RamzinexBuyPrice = Utility::JsonToDouble(jsonRamzinex[map_iter->first]["buy"]);
	// 	map_iter->second.RamzinexSellPrice = Utility::JsonToDouble(jsonRamzinex[map_iter->first]["sell"]);
	// }

	// // cout << "BinanceXRPUSDT: " << mapPrices["xrpusdt"].BinancePrice << ", RamzinexSellXRPUSDT: " << mapPrices["xrpusdt"].RamzinexSellPrice << ", RamzinexBuyXRPUSDT: " << mapPrices["xrpusdt"].RamzinexBuyPrice << endl;
	// // cout << "BinanceBTCUSDT: " << mapPrices["btcusdt"].BinancePrice << ", RamzinexSellBTCUSDT: " << mapPrices["btcusdt"].RamzinexSellPrice << ", RamzinexBuyBTCUSDT: " << mapPrices["btcusdt"].RamzinexBuyPrice << endl;
	// // cout << "BinanceETHUSDT: " << mapPrices["ethusdt"].BinancePrice << ", RamzinexSellETHUSDT: " << mapPrices["ethusdt"].RamzinexSellPrice << ", RamzinexBuyETHUSDT: " << mapPrices["ethusdt"].RamzinexBuyPrice << endl;

	// for (map_iter = mapPrices.begin(); map_iter != mapPrices.end(); map_iter++) {
	// 	if (map_iter->second.RamzinexBuyPrice > map_iter->second.BinancePrice) {
	// 		arbitrageExist = true;
	// 		cout << KRED << "Sell" << RESET << " arbitrage for " << KGRN << map_iter->first << RESET << ", Binance: (" << map_iter->second.BinancePrice << ") and Ramzinex: (" << map_iter->second.RamzinexBuyPrice << ")\n";
	// 	}
	// 	if (map_iter->second.RamzinexSellPrice < map_iter->second.BinancePrice) {
	// 		arbitrageExist = true;
	// 		cout << KRED << "Buy" << RESET << " arbitrage for " << KGRN << map_iter->first << RESET << ", Binance: (" << map_iter->second.BinancePrice << ") and Ramzinex: (" << map_iter->second.RamzinexSellPrice << ")\n";
	// 	}
	// }

	// if (arbitrageExist == false)
	// 	cout << KGRN << "No arbitrage exist between Binance and Ramzinex\n" << RESET;
}

void BotMethod::DollarPriceInRamzinex() {
	Json::Value jsonRamzinex;
	Json::FastWriter fastWriter;

	int lenBinance = 0;
	SymbolPriceSrtuct priceBinance[200];

	string str = "WatchList";
	binance->GetPrices(str, priceBinance, lenBinance);

	str = "All";
	jsonRamzinex = Ramzinex::GetPrices(str);

	double XRPUSDT, BTCUSDT, ETHUSDT, LTCUSDT;

	for (int i=0; i<lenBinance; i++) {
		if (priceBinance[i].symbol == "XRPUSDT")
			XRPUSDT = priceBinance[i].price;
		if (priceBinance[i].symbol == "BTCUSDT") 
			BTCUSDT = priceBinance[i].price;
		if (priceBinance[i].symbol == "ETHUSDT") 
			ETHUSDT = priceBinance[i].price;
		if (priceBinance[i].symbol == "LTCUSDT") 
			LTCUSDT = priceBinance[i].price;
	}

	cout << GREEN("Buy ") << "options for me:\n";
	double SellDollarByUSDT = Utility::JsonToDouble(jsonRamzinex["usdtirr"]["sell"]);
	double SellDollarByXRPUSDT = Utility::JsonToDouble(jsonRamzinex["xrpirr"]["sell"])/XRPUSDT;
	double SellDollarByBTCUSDT = Utility::JsonToDouble(jsonRamzinex["btcirr"]["sell"])/BTCUSDT;
	double SellDollarByETHUSDT = Utility::JsonToDouble(jsonRamzinex["ethirr"]["sell"])/ETHUSDT;
	double SellDollarByLTCUSDT = Utility::JsonToDouble(jsonRamzinex["ltcirr"]["sell"])/LTCUSDT;
	
	double SellMin = 10000000;
	if (SellDollarByUSDT < SellMin)			SellMin = SellDollarByUSDT;
	if (SellDollarByXRPUSDT < SellMin)		SellMin = SellDollarByXRPUSDT;
	if (SellDollarByBTCUSDT < SellMin)		SellMin = SellDollarByBTCUSDT;
	if (SellDollarByETHUSDT < SellMin)		SellMin = SellDollarByETHUSDT;
	if (SellDollarByLTCUSDT < SellMin)		SellMin = SellDollarByLTCUSDT;
	
	cout << "\tusdtirr ----\t" << Utility::JsonToDouble(jsonRamzinex["usdtirr"]["sell"]) << endl;
	cout << "\txrpirr  ----\t" << Utility::JsonToDouble(jsonRamzinex["xrpirr"]["sell"])/XRPUSDT << " (" << jsonRamzinex["xrpirr"]["sell"].asString() << " , " << XRPUSDT << ")" << endl; 
	cout << "\tbtcirr  ----\t" << Utility::JsonToDouble(jsonRamzinex["btcirr"]["sell"])/BTCUSDT << " (" << jsonRamzinex["btcirr"]["sell"].asString() << " , " << BTCUSDT << ")" << endl; 
	cout << "\tethirr  ----\t" << Utility::JsonToDouble(jsonRamzinex["ethirr"]["sell"])/ETHUSDT << " (" << jsonRamzinex["ethirr"]["sell"].asString() << " , " << ETHUSDT << ")" << endl; 
	cout << "\tltcirr  ----\t" << Utility::JsonToDouble(jsonRamzinex["ltcirr"]["sell"])/LTCUSDT << " (" << jsonRamzinex["ltcirr"]["sell"].asString() << " , " << LTCUSDT << ")" << endl; 
	cout << endl << "\tMinimum Buy option: " << SellMin << endl << endl;

	cout << KGRN << "Sell " << RESET << "options for me:\n";
	double BuyDollarByUSDT = Utility::JsonToDouble(jsonRamzinex["usdtirr"]["buy"]);
	double BuyDollarByXRPUSDT = Utility::JsonToDouble(jsonRamzinex["xrpirr"]["buy"])/XRPUSDT;
	double BuyDollarByBTCUSDT = Utility::JsonToDouble(jsonRamzinex["btcirr"]["buy"])/BTCUSDT;
	double BuyDollarByETHUSDT = Utility::JsonToDouble(jsonRamzinex["ethirr"]["buy"])/ETHUSDT;
	double BuyDollarByLTCUSDT = Utility::JsonToDouble(jsonRamzinex["ltcirr"]["buy"])/LTCUSDT;
	
	double BuyMax = 0;
	if (BuyDollarByUSDT > BuyMax)			BuyMax = BuyDollarByUSDT;
	if (BuyDollarByXRPUSDT > BuyMax)		BuyMax = BuyDollarByXRPUSDT;
	if (BuyDollarByBTCUSDT > BuyMax)		BuyMax = BuyDollarByBTCUSDT;
	if (BuyDollarByETHUSDT > BuyMax)		BuyMax = BuyDollarByETHUSDT;
	if (BuyDollarByLTCUSDT > BuyMax)		BuyMax = BuyDollarByLTCUSDT;
	
	cout << "\tusdtirr ----\t" << Utility::JsonToDouble(jsonRamzinex["usdtirr"]["buy"]) << endl;
	cout << "\txrpirr  ----\t" << Utility::JsonToDouble(jsonRamzinex["xrpirr"]["buy"])/XRPUSDT << " (" << jsonRamzinex["xrpirr"]["buy"].asString() << " , " << XRPUSDT << ")" << endl; 
	cout << "\tbtcirr  ----\t" << Utility::JsonToDouble(jsonRamzinex["btcirr"]["buy"])/BTCUSDT << " (" << jsonRamzinex["btcirr"]["buy"].asString() << " , " << BTCUSDT << ")" << endl; 
	cout << "\tethirr  ----\t" << Utility::JsonToDouble(jsonRamzinex["ethirr"]["buy"])/ETHUSDT << " (" << jsonRamzinex["ethirr"]["buy"].asString() << " , " << ETHUSDT << ")" << endl; 
	cout << "\tltcirr  ----\t" << Utility::JsonToDouble(jsonRamzinex["ltcirr"]["buy"])/LTCUSDT << " (" << jsonRamzinex["ltcirr"]["buy"].asString() << " , " << LTCUSDT << ")" << endl; 
	cout << endl << "\tMaximum Sell option: " << BuyMax << endl << endl;
}

static void PrintOrderParam(OrderParams &orderParams) {
	cout << orderParams.pair << " - " << orderParams.side << " - " << orderParams.type << " - " << 
			orderParams.price << " - " << orderParams.stopPrice << " - " << orderParams.origQty << 
			" - " << orderParams.orderId << endl;
}


static int ReadJsonConfigFile(string path, Json::Value &value) {
	ifstream myFile(path);
	if (myFile.is_open() == false) {
		cout << "Cannot open " << path << endl;
		return -1;
	}
	// cout << "Successfully open " << path << endl;

	myFile >> value;
	myFile.close();

	return 0;
}

void BotMethod::Scalping() {
	// // cout << "Inside Scalping\n";
	// double percent = 0.03;

	// // Get current balances
	// map <string, map<string, double>> userBalances = binance->GetBalances(BANK_AND_EXCHANGE_MODE);
	// if (userBalances.size() == 0) {
	// 	cout << "Cannot get userBalances\n";
	// 	return;
	// }
	// // cout << "Successfully GetBalances\n";

	// // Get current prices
	// string str = "All";
	// Json::Value jsonBinance = binance->GetPrices(str);
	// // cout << "Successfully GetPrices\n";
	
	// // Read Scalping parameters
	// Json::Value jsonScalpParams;
	// if (ReadJsonConfigFile("config/ScalpParams.json", jsonScalpParams) < 0) {
	// 	cout << "Cannot read ScalpParams.json\n";
	// 	return;
	// }
	// // cout << jsonScalpParams << endl;

	// // Process
	// for (int i=0; i<jsonScalpParams.size(); i++) {
	// 	// cout << jsonScalpParams[i] << endl;

	// 	int enable = jsonScalpParams[i]["enable"].asInt();
		
	// 	if (enable == 1) {
	// 		string symbol = jsonScalpParams[i]["symbol"].asString();
	// 		string pair = jsonScalpParams[i]["pair"].asString();
	// 		double ScalpingAmount = Utility::JsonToDouble(jsonScalpParams[i]["ScalpingAmount"]);
	// 		string mode = jsonScalpParams[i]["mode"].asString();
	// 		double price = Utility::JsonToDouble(jsonScalpParams[i]["price"]);
	// 		double stopLoss = Utility::JsonToDouble(jsonScalpParams[i]["stopLoss"]);

	// 		// Get balance
	// 		double balance = userBalances[symbol]["f"] + userBalances[symbol]["l"];
			
	// 		// Get price
	// 		double currentPrice = -1;
	// 		for (Json::Value::const_iterator json_iter = jsonBinance.begin(); json_iter != jsonBinance.end(); json_iter++) {
	// 			if ((*json_iter)["symbol"] == pair)
	// 				currentPrice = Utility::JsonToDouble((*json_iter)["price"]);
	// 		}
	// 		if (currentPrice == -1) {
	// 			cout << "Cannot find currentPrice for " << symbol << endl;
	// 			return;
	// 		}

	// 		// log
	// 		// cout << symbol << " is enabled\n";
	// 		// cout << "ScalpingAmount: " << ScalpingAmount << ", price: " << price << ", stopLoss: " << stopLoss << 
	// 		// 		", Free: " << userBalances[symbol]["f"] << ", locked: " << userBalances[symbol]["l"] <<
	// 		// 		", balance: " << balance << ", currentPrice: " << currentPrice << endl;

	// 		// Main part
	// 		OrderParams old_orderParams, new_OrderParams;

	// 		double realScalpingAmount;
	// 		if (mode == "BUY")
	// 			realScalpingAmount = ScalpingAmount - balance;
	// 		else
	// 			realScalpingAmount = ScalpingAmount; 
	// 		RoundAmountBasedOnPair(pair, realScalpingAmount);

	// 		if (_FillOldOrderParams("Binance", pair, realScalpingAmount, old_orderParams) >= 0) {
	// 			if (mode == "BUY") {
	// 				if ((currentPrice*(1-percent)) < price) {
	// 					cout << "Price of " << YELLOW(pair) << " is trigger low for " << YELLOW("buy") << 
	// 							" (" << RED(currentPrice) << "). Price: " << RED(price) << endl;
						
	// 					new_OrderParams.pair = pair;
	// 					new_OrderParams.side = "BUY";
	// 					new_OrderParams.type = "LIMIT";
	// 					new_OrderParams.price = price;
	// 					new_OrderParams.stopPrice = 0;				// don't care
	// 					new_OrderParams.origQty = realScalpingAmount;
	// 					new_OrderParams.orderId = 0;				// don't care

	// 					_handleNewTrig(old_orderParams, new_OrderParams);
	// 				}
	// 				if ((currentPrice*(1+percent)) > stopLoss) {
	// 					cout << "Price of " << YELLOW(pair) << " is trigger stop_loss for " << YELLOW("buy") << 
	// 							" (" << RED(currentPrice) << "). StopLoss: " << RED(stopLoss) << endl;

	// 					// new_OrderParams.pair = pair;
	// 					// new_OrderParams.side = "BUY";
	// 					// new_OrderParams.type = "LIMIT";
	// 					// new_OrderParams.price = price;
	// 					// new_OrderParams.stopPrice = 0;				// don't care
	// 					// new_OrderParams.origQty = realScalpingAmount;
	// 					// new_OrderParams.orderId = 0;				// don't care

	// 					// _handleNewTrig(old_orderParams, new_OrderParams);
	// 				}			
	// 			}
	// 			else if (mode == "SELL") {
	// 				if ((currentPrice*(1+percent)) > price) {
	// 					cout << "Price of " << YELLOW(pair) << " is trigger high for " << YELLOW("sell") << 
	// 							" (" << RED(currentPrice) << "). Price: " << RED(price) << endl;
					
	// 					new_OrderParams.pair = pair;
	// 					new_OrderParams.side = "SELL";
	// 					new_OrderParams.type = "LIMIT";
	// 					new_OrderParams.price = price;
	// 					new_OrderParams.stopPrice = 0;				// don't care
	// 					new_OrderParams.origQty = realScalpingAmount;
	// 					new_OrderParams.orderId = 0;				// don't care

	// 					_handleNewTrig(old_orderParams, new_OrderParams);
	// 				}
	// 				if ((currentPrice*(1-percent)) < stopLoss) {
	// 					cout << "Price of " << YELLOW(pair) << " is trigger stop_loss for " << YELLOW("sell") << 
	// 							" (" << RED(currentPrice) << "). StopLoss: " << RED(stopLoss) << endl;
	// 					// new_OrderParams.pair = pair;
	// 					// new_OrderParams.side = "SELL";
	// 					// new_OrderParams.type = "LIMIT";
	// 					// new_OrderParams.price = price;
	// 					// new_OrderParams.stopPrice = 0;				// don't care
	// 					// new_OrderParams.origQty = realScalpingAmount;
	// 					// new_OrderParams.orderId = 0;				// don't care

	// 					// _handleNewTrig(old_orderParams, new_OrderParams);
	// 				}
	// 			}
	// 			else
	// 				cout << RED("Invalid mode " + mode) << endl;
	// 		}
	// 	}
	// }
}

void BotMethod::Trading() {HodlingTradingHandler(ONLY_TRADING_MODE);}

void BotMethod::Hodling() {HodlingTradingHandler(ONLY_HODLING_MODE);}

void BotMethod::HodlingTrading() {HodlingTradingHandler(HODLING_AND_TRADING_MODE);}

void BotMethod::HodlingTradingHandler(_BotHodlingTradingModes mode) {
	// cout << "Inside TradingHodling\n";
	double percent = 0.025;

	// Get current prices
	string str = "WatchList";
	SymbolPriceSrtuct priceBinance[200];
	int lenBinance;
	binance->GetPrices(str, priceBinance, lenBinance);
	cout << "Successfully GetPrices from Binance\n";

	str = "WatchList";
	SymbolPriceSrtuct priceCoinex[200];
	int lenCoinex;
	coinex->GetPrices(str, priceCoinex, lenCoinex);
	cout << "Successfully GetPrices from Coinex\n";

	str = "WatchList";
	SymbolPriceSrtuct priceHitBTC[200];
	int lenHitBTC;
	hitbtc->GetPrices(str, priceHitBTC, lenHitBTC);
	cout << "Successfully GetPrices from HitBTC\n";

	str = "WatchList";
	SymbolPriceSrtuct priceKucoin[200];
	int lenKucoin;
	kucoin->GetPrices(str, priceKucoin, lenKucoin);
	cout << "Successfully GetPrices from Kucoin\n";


	// cout << "Successfully GetPrices\n";
	
	// Read hodling parameters
	Json::Value jsonHoldingParams;
	if (ReadJsonConfigFile("config/HodlingParams.json", jsonHoldingParams) < 0) {
		cout << "Cannot read HodlingParams.json\n";
		return;
	}
	cout << "Successfully read json file\n";
	// cout << jsonHoldingParams << endl;

	// Process
	list <HodlingParams> historyList;

	double sumBoughtPrice = 0, sumCurrentPrice = 0;
	for (int i=0; i<jsonHoldingParams.size(); i++) {
		// cout << jsonHoldingParams[i] << endl;
		HodlingParams history;

		history.symbol = jsonHoldingParams[i]["symbol"].asString();
		history.pair = jsonHoldingParams[i]["pair"].asString();
		string date = jsonHoldingParams[i]["date"].asString();
		history.hodlingAmount = Utility::JsonToDouble(jsonHoldingParams[i]["hodlingAmount"]);
		history.priceBuy = Utility::JsonToDouble(jsonHoldingParams[i]["priceBuy"]);
		double priceHigh = Utility::JsonToDouble(jsonHoldingParams[i]["priceHigh"]);
		double priceStopLoss = Utility::JsonToDouble(jsonHoldingParams[i]["priceStopLoss"]);
		history.storedIn = jsonHoldingParams[i]["storedIn"].asString();
		history.mode = jsonHoldingParams[i]["mode"].asString();

		_BotHodlingTradingModes jsonMode;
		if (history.mode == "Hodling")
			jsonMode = ONLY_HODLING_MODE;
		else if (history.mode == "Trading")
			jsonMode = ONLY_TRADING_MODE;
		else 
			jsonMode = HODLING_AND_TRADING_MODE;

		if (mode == HODLING_AND_TRADING_MODE || jsonMode == mode) {
			// Get price
			double price = -1;
			if (history.storedIn == "Binance" || history.storedIn == "Wallet") {
				for (int i=0; i<lenBinance; i++)
					if (priceBinance[i].symbol == history.pair)
						price = priceBinance[i].price;
			}
			else if (history.storedIn == "Coinex") {
				for (int i=0; i<lenCoinex; i++)
					if (priceCoinex[i].symbol == history.pair)
						price = priceCoinex[i].price;
			}
			else if (history.storedIn == "HitBTC") {
				for (int i=0; i<lenHitBTC; i++)
					if (priceHitBTC[i].symbol == history.pair)
						price = priceHitBTC[i].price;
			}
			else {
				cout << "Invalid storedIn destination " << YELLOW(history.storedIn) << endl;
				return;
			}
			history.priceCurrent = price;
			historyList.push_back(history);

			// log
			cout << "Symbol: " << YELLOWw(history.symbol, 5) << ",  " << setw(12) << date << ", amount: " << REDw(history.hodlingAmount, 6) << ",\t" << YELLOW(history.storedIn) << endl;
			if (price > history.priceBuy)
				cout << GREEN(history.hodlingAmount * (price - history.priceBuy)) << " ( " << GREEN(((price - history.priceBuy)/history.priceBuy)*100) << GREEN(" %") << " )";
			else
				cout << RED(history.hodlingAmount * (price - history.priceBuy)) << " (" << RED(((price - history.priceBuy)/history.priceBuy)*100) << RED(" %") << " )";
			cout << "\tBuy: " << YELLOW(history.priceBuy) << " (" << YELLOW(history.hodlingAmount*history.priceBuy*1.001) << "),\tCurrent: " << 
					YELLOW(price) << " (" << YELLOW(history.hodlingAmount*price*0.999) << "),\tHigh: " << 
					YELLOW(priceHigh) << ",\tStopLoss: " << RED(priceStopLoss) << endl << endl;


			// Check prices
			OrderParams old_orderParams, new_OrderParams;
			// if (_FillOldOrderParams(history.storedIn, pair, history.hodlingAmount, old_orderParams) >= 0) {
				if ((price*(1+percent)) > priceHigh) {
					cout << "Price of " << YELLOW(history.pair) << " is trigger high (" << RED(price) << 
							"). PriceHigh: " << RED(priceHigh) << endl;

					// new_OrderParams.pair = pair;
					// new_OrderParams.side = "SELL";
					// new_OrderParams.type = "LIMIT";
					// new_OrderParams.price = priceHigh;
					// new_OrderParams.stopPrice = 0;				// don't care
					// new_OrderParams.origQty = hodlingAmount;
					// new_OrderParams.orderId = 0;				// don't care

					// _handleNewTrig(old_orderParams, new_OrderParams);
				}

				if ((price*(1-percent)) < priceStopLoss) {
					cout << "Price of " << YELLOW(history.pair) << " is trigger stop_loss (" << RED(price) << 
							"). PriceStopLoss: " << RED(priceStopLoss) << endl;

					// new_OrderParams.pair = pair;
					// new_OrderParams.side = "SELL";
					// new_OrderParams.type = "STOP_LOSS_LIMIT";
					// new_OrderParams.price = priceStopLoss;
					// new_OrderParams.stopPrice = priceStopLoss;
					// new_OrderParams.origQty = hodlingAmount;
					// new_OrderParams.orderId = 0;				// don't care

					// _handleNewTrig(old_orderParams, new_OrderParams);
				}
			// }

			sumBoughtPrice += history.hodlingAmount*history.priceBuy*1.001;
			sumCurrentPrice += history.hodlingAmount*price*0.999;
		}
	}
	// cout << "\nIn summary, I bought " << RED(sumBoughtPrice) << 
	// 		", and it's worth " << GREEN(sumCurrentPrice) << " now.\n\n";
	cout << "\nSummary:    bought " << RED(sumBoughtPrice) << ",    current " << GREEN(sumCurrentPrice) << ",    ";
	if (sumCurrentPrice > sumBoughtPrice)
		cout << GREEN(sumCurrentPrice - sumBoughtPrice) << " ( " << GREEN((sumCurrentPrice - sumBoughtPrice)/sumBoughtPrice*100) << GREEN(" %") << " )\n";
	else
		cout << RED(sumCurrentPrice - sumBoughtPrice) << " ( " << RED((sumCurrentPrice - sumBoughtPrice)/sumBoughtPrice*100) << RED(" %") << " )\n";

	cout << endl << endl;
	map <string, prices> exchanges;
	map <string, prices> coins;
	for (list<HodlingParams>::iterator iter = historyList.begin(); iter != historyList.end(); iter ++) {
		exchanges[iter->storedIn].buy = 0;
		exchanges[iter->storedIn].current = 0;
		coins[iter->symbol].buy = 0;
		coins[iter->symbol].current = 0;
	}
	for (list<HodlingParams>::iterator iter = historyList.begin(); iter != historyList.end(); iter ++) { 
		exchanges[iter->storedIn].buy += (iter->priceBuy*iter->hodlingAmount)*1.001;
		exchanges[iter->storedIn].current += (iter->priceCurrent*iter->hodlingAmount)*0.999;
		coins[iter->symbol].buy += (iter->priceBuy*iter->hodlingAmount)*1.001;
		coins[iter->symbol].current += (iter->priceCurrent*iter->hodlingAmount)*0.999;
	}
	vector <pair<string, prices>> coinsSorted;
	sortMapBasedOnPriceCurrent(coins, coinsSorted);
	vector <pair<string, prices>> exchangesSorted;
	sortMapBasedOnPriceCurrent(exchanges, exchangesSorted);

	for (vector <pair<string, prices>>::iterator iter = exchangesSorted.begin(); iter != exchangesSorted.end(); iter ++) {
		// cout << "Exchange: " << KYEL << setw(7) << iter->first << RESET << ",  bought: " << KRED << setw (10) << iter->second.buy << RESET << ",  current: " << KGRN << setw(10) << iter->second.current << RESET << ",  ";
		cout << "Exchange: " << YELLOWw(iter->first, 7) << ",  bought: " << REDw(iter->second.buy, 10) << ",  current: " << GREENw(iter->second.current, 10) << ",  ";
		if (iter->second.current > iter->second.buy)
			cout << GREENw(iter->second.current - iter->second.buy, 10) << " ( " << GREEN((iter->second.current - iter->second.buy)/iter->second.buy*100) << GREEN(" %") << " )\n";
		else
			cout << REDw(iter->second.current - iter->second.buy, 10) << " ( " << RED((iter->second.current - iter->second.buy)/iter->second.buy*100) << RED(" %") << " )\n";
	}

	cout << endl;

	for (vector<pair<string, prices>>::iterator iter = coinsSorted.begin(); iter != coinsSorted.end(); iter ++) {
		cout << "Symbol: " << YELLOWw(iter->first, 5) << ",  bought: " << REDw(iter->second.buy, 10) <<  ",  current: " << GREENw(iter->second.current, 10) << ",  ";
		if (iter->second.current > iter->second.buy)
			cout << GREENw(iter->second.current - iter->second.buy, 10) << " ( " << GREEN((iter->second.current - iter->second.buy)/iter->second.buy*100) << GREEN(" %") << " )\n";
		else
			cout << REDw(iter->second.current - iter->second.buy, 10) << " ( " << RED((iter->second.current - iter->second.buy)/iter->second.buy*100) << RED(" %") << " )\n";
	}
}

void BotMethod::Scalping2() {
	// cout << "Inside Scalping2\n";
	// double decisionPercent = 0.01;
	// double sellPercent = 0.01;
	// double stopLossPercent = 0.04;

	// // Get current balances
	// map <string, map<string, double>> userBalances = binance->GetBalances(BANK_AND_EXCHANGE_MODE);
	// if (userBalances.size() == 0) {
	// 	cout << "Cannot get userBalances\n";
	// 	return;
	// }
	// // cout << "Successfully GetBalances\n";

	// // Get current prices
	// string str = "All";
	// Json::Value jsonBinance = binance->GetPrices(str);
	// // cout << "Successfully GetPrices\n";
	
	// // Read Scalping parameters
	// Json::Value jsonScalpParams;
	// if (ReadJsonConfigFile("config/ScalpParams2.json", jsonScalpParams) < 0) {
	// 	cout << "Cannot read ScalpParams.json\n";
	// 	return;
	// }
	// // cout << jsonScalpParams << endl;

	// // Process
	// for (int i=0; i<jsonScalpParams.size(); i++) {
	// 	// cout << jsonScalpParams[i] << endl;

	// 	int enable = jsonScalpParams[i]["enable"].asInt();
		
	// 	if (enable == 1) {
	// 		string symbol = jsonScalpParams[i]["symbol"].asString();
	// 		string pair = jsonScalpParams[i]["pair"].asString();
	// 		double ScalpingAmount = Utility::JsonToDouble(jsonScalpParams[i]["ScalpingAmount"]);
	// 		double price = Utility::JsonToDouble(jsonScalpParams[i]["price"]);

	// 		// Get balance
	// 		double balance = userBalances[symbol]["f"] + userBalances[symbol]["l"];
			
	// 		// Get price
	// 		double currentPrice = -1;
	// 		for (Json::Value::const_iterator json_iter = jsonBinance.begin(); json_iter != jsonBinance.end(); json_iter++) {
	// 			if ((*json_iter)["symbol"] == pair)
	// 				currentPrice = Utility::JsonToDouble((*json_iter)["price"]);
	// 		}
	// 		if (currentPrice == -1) {
	// 			cout << "Cannot find currentPrice for " << symbol << endl;
	// 			return;
	// 		}

	// 		// log
	// 		cout << symbol << " is enabled\n";
	// 		cout << "ScalpingAmount: " << ScalpingAmount << ", price: " << price <<
	// 				", Free: " << userBalances[symbol]["f"] << ", locked: " << userBalances[symbol]["l"] <<
	// 				", balance: " << balance << ", currentPrice: " << currentPrice << endl;

	// 		// Main part
	// 		OrderParams old_orderParams, new_OrderParams;

	// 		if (_FillOldOrderParams("Binance", pair, ScalpingAmount, old_orderParams) >= 0) {
	// 			double sellPrice = price * (1+sellPercent);
	// 			RoundPriceBasedOnPair(pair, sellPrice);
	// 			double stopLossPrice = price * (1-stopLossPercent);
	// 			RoundPriceBasedOnPair(pair, stopLossPrice);
	// 			cout << "price: " << price << ", sellPrice: " << sellPrice << ", stopLossPrice: " << stopLossPrice << endl;

	// 			string mode;
	// 			if (abs(ScalpingAmount - balance) > 0.1*ScalpingAmount)		// ????
	// 				mode = "BUY";
	// 			else 
	// 				mode = "SELL";

	// 			new_OrderParams.pair = pair;
	// 			new_OrderParams.side = mode;
	// 			new_OrderParams.orderId = 0;				// don't care
				
	// 			if (mode == "BUY") {
	// 				new_OrderParams.type = "LIMIT";
	// 				new_OrderParams.price = price;
	// 				new_OrderParams.stopPrice = 0;				// don't care
	
	// 				double realScalpingAmount = ScalpingAmount - balance;
	// 				RoundAmountBasedOnPair(pair, realScalpingAmount);
	// 				new_OrderParams.origQty = realScalpingAmount;

	// 				_handleNewTrig(old_orderParams, new_OrderParams);
	// 			}
	// 			else if (mode == "SELL") {
					
	// 				RoundAmountBasedOnPair(pair, ScalpingAmount);
					
	// 				if (currentPrice * (1+decisionPercent) > sellPrice) {
	// 					cout << "Price of " << YELLOW(pair) << " is trigger high sell (" << RED(currentPrice) << 
	// 							"). sellPrice: " << RED(sellPrice) << endl;
						
	// 					new_OrderParams.type = "LIMIT";
	// 					new_OrderParams.price = sellPrice;
	// 					new_OrderParams.stopPrice = 0;				// don't care
	// 					new_OrderParams.origQty = ScalpingAmount;

	// 					_handleNewTrig(old_orderParams, new_OrderParams);
	// 				}

	// 				if (currentPrice * (1-decisionPercent) < stopLossPrice) {
	// 					cout << "Price of " << YELLOW(pair) << " is trigger stop_loss sell (" << RED(currentPrice) << 
	// 							"). stopLossPrice: " << RED(stopLossPrice) << endl;
						
	// 					// new_OrderParams.type = "LIMIT";
	// 					// new_OrderParams.price = sellPrice;
	// 					// new_OrderParams.stopPrice = 0;				// don't care
	// 					// new_OrderParams.origQty = ScalpingAmount;

	// 					// _handleNewTrig(old_orderParams, new_OrderParams);
	// 				}
	// 			}
	// 			else {
	// 				cout << "It's not possible to come here\n";
	// 				return;
	// 			}


	// 			// if ((currentPrice*(1-percent)) < price) {
	// 			// 	cout << "Price of " << YELLOW(pair) << " is trigger low for " << YELLOW("buy") << 
	// 			// 			" (" << RED(currentPrice) << "). Price: " << RED(price) << endl;
					
	// 			// 	new_OrderParams.pair = pair;
	// 			// 	new_OrderParams.side = "BUY";
	// 			// 	new_OrderParams.type = "LIMIT";
	// 			// 	new_OrderParams.price = price;
	// 			// 	new_OrderParams.stopPrice = 0;				// don't care
	// 			// 	new_OrderParams.origQty = realScalpingAmount;
	// 			// 	new_OrderParams.orderId = 0;				// don't care

	// 			// 	_handleNewTrig(old_orderParams, new_OrderParams);
	// 			// }
	// 		}
	// 	}
	// }
}

void BotMethod::RandomBuy() {
	double money;
	cout << "Enter money: \n";
	cin >> money;

	// if (money > 0) {
	// 	string pair = Menu::binancePairSubMenuList();

	// 	SymbolPriceSrtuct priceBinance;
	// 	int lenBinance= 0;
	// 	binance->GetPrices(pair, &priceBinance, lenBinance);

	// 	double quantity = money/priceBinance.price;
	// 	Utility::RoundAmountBasedOnPair(pair, quantity);

	// 	cout << "Estimated price: " << priceBinance.price << ", quantity: " << quantity << endl;
	// 	binance->SendOrder(pair, "BUY", "MARKET", quantity, 0, 0, 0);

	// 	double price = priceBinance.price * 1.011;
	// 	double stopPrice = priceBinance.price * 0.97;
	// 	double stopLimitPrice = priceBinance.price * 0.96;

	// 	cout << "quantity: " << quantity << ", priceBuy: " << priceBinance.price << 
	// 			", price: " << price << ", stopPrice: " << stopPrice << ", stopLimitPrice: " << stopLimitPrice << endl;
		
	// 	Utility::RoundPriceBasedOnPair(pair, price);
	// 	Utility::RoundPriceBasedOnPair(pair, stopPrice);
	// 	Utility::RoundPriceBasedOnPair(pair, stopLimitPrice);

	// 	cout << "quantity: " << quantity << ", priceBuy: " << priceBinance.price << 
	// 			", price: " << price << ", stopPrice: " << stopPrice << ", stopLimitPrice: " << stopLimitPrice << endl;
		
	// 	binance->SendOrder(pair, "SELL", "OCO", quantity, price, stopPrice, stopLimitPrice);
	// }

	if (money > 0) {
		string pair = Menu::binancePairSubMenuList();

		double priceBuy;
		cout << "Enter priceBuy: \n";
		cin >> priceBuy;

		if (priceBuy > 0) {
			double quantity = money/priceBuy;
			Utility::RoundAmountBasedOnPair(pair, quantity);

			cout << "priceBuy: " << priceBuy << ", quantity: " << quantity << endl;
			binance->SendOrder(pair, "BUY", "LIMIT", quantity, priceBuy, 0, 0);

			map <string, map<string,double>> userBalance;
			bool waitToBuy = true;
			while (waitToBuy) {
				userBalance = binance->GetBalances(BANK_AND_EXCHANGE_MODE);
				for (map < string, map<string,double> >::iterator it_i=userBalance.begin(); it_i!=userBalance.end(); it_i++) {
					if ((*it_i).first == pair && ((*it_i).second["f"] != 0 || (*it_i).second["l"] != 0)) {
						cout << GREEN(pair) << " is bought. quantity: " << (*it_i).second["f"] + (*it_i).second["l"] << endl;
						waitToBuy = false;
					}
				}
				if (waitToBuy)
					sleep(120);
			}

			double price = priceBuy * 1.011;
			double stopPrice = priceBuy * 0.97;
			double stopLimitPrice = priceBuy * 0.96;

			cout << "quantity: " << quantity << ", priceBuy: " << priceBuy << 
					", price: " << price << ", stopPrice: " << stopPrice << ", stopLimitPrice: " << stopLimitPrice << endl;
			
			Utility::RoundPriceBasedOnPair(pair, price);
			Utility::RoundPriceBasedOnPair(pair, stopPrice);
			Utility::RoundPriceBasedOnPair(pair, stopLimitPrice);

			cout << "quantity: " << quantity << ", priceBuy: " << priceBuy << 
					", price: " << price << ", stopPrice: " << stopPrice << ", stopLimitPrice: " << stopLimitPrice << endl;
			
			binance->SendOrder(pair, "SELL", "OCO", quantity, price, stopPrice, stopLimitPrice);
		}
	}
}

void BotMethod::CancelAllOrders(string &str, string exchange) {
	// cout << "Inside CancelAllOrders " << str << "\n";

	if (exchange == "BINANCE") {
		Json::Value jsonOpenOrders;
		if (binance->GetOpenOrders(str, jsonOpenOrders) == false) {
			cout << "Cannot GetOpenOrders\n";
			return;
		}

		for (int i=0 ; i<jsonOpenOrders.size(); i++)
			binance->CancelOrder(jsonOpenOrders[i]["symbol"].asString(), jsonOpenOrders[i]["orderId"].asInt64());
	}
	else if (exchange == "COINEX") {
		vector <SymbolOrderStruct> vecOpenOrders;
		if (coinex->GetOpenOrders(str, vecOpenOrders) == false) {
			cout << "Cannot GetOpenOrders\n";
			return;
		}

		for (vector<SymbolOrderStruct>::iterator iter = vecOpenOrders.begin(); iter != vecOpenOrders.end(); iter++)
			coinex->CancelOrder(str, iter->id);
	}
	else if (exchange == "HITBTC") {
		Json::Value jsonOpenOrders;
		if (hitbtc->GetOpenOrders(str, jsonOpenOrders) == false) {
			cout << "Cannot GetOpenOrders\n";
			return;
		}

		for (int i=0 ; i<jsonOpenOrders.size(); i++)
			hitbtc->CancelOrder(jsonOpenOrders[i]["symbol"].asString(), jsonOpenOrders[i]["clientOrderId"].asString());
	}
	else if (exchange == "KUCOIN") 
		kucoin->CancelAllOrders(str);
	else {
		cout << "Invalid exchange " << exchange << endl;
		return;
	}
	exit(0);
}

void BotMethod::SetJsonFileOrders() {
	// cout << "Inside SetJsonFileOrders\n";
	
	// Read Scalping parameters
	Json::Value jsonOrders;
	if (ReadJsonConfigFile("config/Orders.json", jsonOrders) < 0) {
		cout << "Cannot read Orders.json\n";
		return;
	}
	// cout << jsonOrders << endl;

	// Process
	for (int i=0; i<jsonOrders.size(); i++) {
		// cout << jsonOrders[i] << endl;

		int enable = jsonOrders[i]["enable"].asInt();
		
		if (enable == 1) {
			string symbol = jsonOrders[i]["symbol"].asString();
			string pair = jsonOrders[i]["pair"].asString();
			double orderAmount = Utility::JsonToDouble(jsonOrders[i]["orderAmount"]);
			string mode = jsonOrders[i]["mode"].asString();
			double price = Utility::JsonToDouble(jsonOrders[i]["price"]);
			string exchange = jsonOrders[i]["exchange"].asString();
			string type = jsonOrders[i]["type"].asString();

			// log
			// cout << symbol << " is enabled\n";
			// cout << "ScalpingAmount: " << ScalpingAmount << ", price: " << price << ", stopLoss: " << stopLoss << 
			// 		", Free: " << userBalances[symbol]["f"] << ", locked: " << userBalances[symbol]["l"] <<
			// 		", balance: " << balance << ", currentPrice: " << currentPrice << endl;

			// Main part
			OrderParams old_orderParams, new_OrderParams;

			Utility::RoundAmountBasedOnPair(pair, orderAmount);

			if (_FillOldOrderParams(exchange, pair, orderAmount, old_orderParams) >= 0) {
				new_OrderParams.pair = pair;
				new_OrderParams.side = mode;
				new_OrderParams.type = type;
				new_OrderParams.price = price;
				new_OrderParams.stopPrice = 0;				// don't care
				new_OrderParams.origQty = orderAmount;
				new_OrderParams.orderId = 0;				// don't care

				_handleNewTrig(old_orderParams, new_OrderParams);
			}
		}
	}
}

int BotMethod::_FillOldOrderParams(string storedIn, string pair, double amount, OrderParams &orderParams) {
	// cout << "Inside _FillOldOrderParams\n";
	
	orderParams = {};	// reset structure values

	if (amount <= 0) {
		cout << "_FillOldOrderParams for " << pair << ", amount is less than 0\n";
		return -1;
	}

	if (storedIn == "Binance") {
		Json::Value jsonOpenOrders;
		if (binance->GetOpenOrders(pair, jsonOpenOrders)) {
			for (int i=0; i<jsonOpenOrders.size(); i++) {
				// cout << jsonOpenOrders[i]["symbol"].asString() << " - " << pair << " - " << 
				// 		amount << " - " << Utility::JsonToDouble(jsonOpenOrders[i]["origQty"]) << endl;
				if (jsonOpenOrders[i]["symbol"].asString() == pair &&
					(Utility::JsonToDouble(jsonOpenOrders[i]["origQty"]) - amount) < 0.0001) {
					// cout << "An order exist for " << pair <<" with amount " << amount << endl;	

					orderParams.pair = pair;
					orderParams.side = jsonOpenOrders[i]["side"].asString();
					orderParams.type = jsonOpenOrders[i]["type"].asString();
					orderParams.price = Utility::JsonToDouble(jsonOpenOrders[i]["price"]);
					orderParams.stopPrice = Utility::JsonToDouble(jsonOpenOrders[i]["stopPrice"]);
					orderParams.origQty = Utility::JsonToDouble(jsonOpenOrders[i]["origQty"]);
					orderParams.orderId = Utility::JsonToLong(jsonOpenOrders[i]["orderId"]);
				}
			}
		}
		else {
			cout << "Cannot GetOpenOrders for " << YELLOW(pair) << endl;	
			return -1;
		}
	}
	return 0;
}

void BotMethod::_removeAnOrder(OrderParams &orderParams) {
	// cout << "Inside _removeAnOrder\n";

	// void binance->CancelOrder(string symbol, long orderId);
	cout << "We shoule remove an order of " << orderParams.pair << "with price " << orderParams.price <<
			"and origQty " << orderParams.origQty << ". OrderId " << orderParams.orderId << endl;

	cout << "Issue binance->CancelOrder(" << orderParams.pair << ", " << orderParams.orderId << ");\n";
	binance->CancelOrder(orderParams.pair, orderParams.orderId);
	cout << GREEN("CancelOrder is Issued\n");
}

void BotMethod::_addLimitOrder(OrderParams &orderParams) {
	// cout << "Inside _addLimitOrder\n";

	// void binance->SendOrder(string symbol, string side, string type, double quantity, double price);
	cout << "We shoule add limit order for " << orderParams.pair << " with price " << orderParams.price <<
			" and origQty " << orderParams.origQty << endl;

	cout << "Issue binance->SendOrder(" << orderParams.pair << ", " << orderParams.side << ", " <<
			orderParams.type << ", " << orderParams.origQty << ", " << orderParams.price << ");\n";
	binance->SendOrder(orderParams.pair, orderParams.side, orderParams.type, orderParams.origQty, orderParams.price, 0, 0);
	cout << GREEN("SendOrder is Issued\n");
}

// stop loss order is not working!
void BotMethod::_addStopLossOrder(OrderParams &orderParams) {
	// cout << "Inside _addStopLossOrder\n";

	// void binance->SendOrder(string symbol, string side, string type, double quantity, double price);
	cout << "We shoule add stop_loss order for " << orderParams.pair << " with price " << orderParams.price <<
			" and origQty " << orderParams.origQty << endl;

	cout << "Issue binance->SendOrder(" << orderParams.pair << ", " << orderParams.side << ", " <<
			orderParams.type << ", " << orderParams.origQty << ", " << orderParams.price << ");\n";
}

bool BotMethod::_CompareOrderParams(OrderParams &old_orderParams, OrderParams &new_orderParams) {

	if ((old_orderParams.side == new_orderParams.side) && 
		(old_orderParams.type == new_orderParams.type) && 
		((abs(old_orderParams.price - new_orderParams.price)) < 0.0001) && 
		((abs(old_orderParams.origQty - new_orderParams.origQty)) < 0.0001))
		return true;
	else
		return false;
}

void BotMethod::_handleNewTrig(OrderParams &old_orderParams, OrderParams &new_orderParams) {
	cout << "old_orderParams:\n\t";
	PrintOrderParam(old_orderParams);
	cout << "new_orderParams:\n\t";
	PrintOrderParam(new_orderParams);

	if (_CompareOrderParams(old_orderParams, new_orderParams))
		return;

	if (old_orderParams.orderId != 0) 
		_removeAnOrder(old_orderParams);
	if (new_orderParams.type == "LIMIT")
		_addLimitOrder(new_orderParams);
	else if (new_orderParams.type == "STOP_LOSS_LIMIT") 
		_addStopLossOrder(new_orderParams);
	else
		cout << RED("Invalid type" + new_orderParams.type) << endl;	
}

void BotMethod::CheckHistoryTrading() {CheckHistoryHodlingTradingHandler(ONLY_TRADING_MODE);}

void BotMethod::CheckHistoryHodling() {CheckHistoryHodlingTradingHandler(ONLY_HODLING_MODE);}

void BotMethod::CheckHistoryHodlingTrading() {CheckHistoryHodlingTradingHandler(HODLING_AND_TRADING_MODE);}

void BotMethod::CheckHistoryHodlingTradingHandler(_BotHodlingTradingModes mode) {
	// cout << "Inside CheckHistoryHodlingTradingHandler\n";

	// Get current prices
	string str = "WatchList";
	SymbolPriceSrtuct priceBinance[200];
	int lenBinance;
	binance->GetPrices(str, priceBinance, lenBinance);
	cout << "Successfully GetPrices from Binance\n";

	str = "WatchList";
	SymbolPriceSrtuct priceCoinex[200];
	int lenCoinex;
	coinex->GetPrices(str, priceCoinex, lenCoinex);
	cout << "Successfully GetPrices from Coinex\n";

	str = "WatchList";
	SymbolPriceSrtuct priceHitBTC[200];
	int lenHitBTC;
	hitbtc->GetPrices(str, priceHitBTC, lenHitBTC);
	cout << "Successfully GetPrices from HitBTC\n";

	str = "WatchList";
	SymbolPriceSrtuct priceKucoin[200];
	int lenKucoin;
	kucoin->GetPrices(str, priceKucoin, lenKucoin);
	cout << "Successfully GetPrices from Kucoin\n";


	// cout << "Successfully GetPrices\n";
	
	// Read hodling parameters
	Json::Value jsonHoldingHistory;
	if (ReadJsonConfigFile("config/HodlingHistory.json", jsonHoldingHistory) < 0) {
		cout << "Cannot read HodlingHistory.json\n";
		return;
	}
	cout << "Successfully read json file\n";
	// cout << jsonHoldingHistory << endl;

	// Process
	list <HodlingParams> historyList;
	double sumBoughtPrice = 0, sumSoldPrice = 0, sumCurrentPrice = 0;
	for (int i=0; i<jsonHoldingHistory.size(); i++) {
		// cout << jsonHoldingHistory[i] << endl;
		HodlingParams history;

		history.symbol = jsonHoldingHistory[i]["symbol"].asString();
		history.pair = jsonHoldingHistory[i]["pair"].asString();
		history.hodlingAmount = Utility::JsonToDouble(jsonHoldingHistory[i]["hodlingAmount"]);
		string dateBuy = jsonHoldingHistory[i]["dateBuy"].asString();
		history.priceBuy = Utility::JsonToDouble(jsonHoldingHistory[i]["priceBuy"]);
		string dateSell = jsonHoldingHistory[i]["dateSell"].asString();
		history.priceSell = Utility::JsonToDouble(jsonHoldingHistory[i]["priceSell"]);
		history.storedIn = jsonHoldingHistory[i]["storedIn"].asString();
		history.mode = jsonHoldingHistory[i]["mode"].asString();

		_BotHodlingTradingModes jsonMode;
		if (history.mode == "Hodling")
			jsonMode = ONLY_HODLING_MODE;
		else if (history.mode == "Trading")
			jsonMode = ONLY_TRADING_MODE;
		else 
			jsonMode = HODLING_AND_TRADING_MODE;

		if (mode == HODLING_AND_TRADING_MODE || jsonMode == mode) {
			historyList.push_back(history);

			// Get price
			double price = -1;
			if (history.storedIn == "Binance" || history.storedIn == "Wallet") {
				for (int i=0; i<lenBinance; i++)
					if (priceBinance[i].symbol == history.pair)
						price = priceBinance[i].price;
			}
			else if (history.storedIn == "Coinex") {
				for (int i=0; i<lenCoinex; i++)
					if (priceCoinex[i].symbol == history.pair)
						price = priceCoinex[i].price;
			}
			else if (history.storedIn == "HitBTC") {
				for (int i=0; i<lenHitBTC; i++)
					if (priceHitBTC[i].symbol == history.pair)
						price = priceHitBTC[i].price;
			}
			else if (history.storedIn == "Kucoin") {
				for (int i=0; i<lenKucoin; i++)
					if (priceKucoin[i].symbol == history.pair)
						price = priceKucoin[i].price;
			}
			else {
				cout << "Invalid storedIn destination " << YELLOW(history.storedIn) << endl;
				return;
			}

			// log
			cout << "Symbol: " << YELLOWw(history.symbol, 5) << ",   amount: " << REDw(history.hodlingAmount, 5) << ",    " << YELLOWw(history.storedIn, 8) << ",    dateBuy: " << setw(15) << dateBuy << ",    dateSell: " << setw(15) <<  dateSell << endl;
			if (history.priceSell > history.priceBuy)
				cout << GREENw(history.hodlingAmount * (history.priceSell - history.priceBuy), 8) << " ( " << GREENw(((history.priceSell - history.priceBuy)/history.priceBuy)*100, 8) << GREEN(" %") << " )";
			else
				cout << REDw(history.hodlingAmount * (history.priceSell - history.priceBuy), 8) << " (" << REDw(((history.priceSell - history.priceBuy)/history.priceBuy)*100, 8) << RED(" %") << " )";
			cout << "\tBought: " << YELLOWw(history.priceBuy, 8) << " (" << YELLOWw(history.hodlingAmount*history.priceBuy*1.001, 8) << 
					"),\tSold: " << YELLOWw(history.priceSell, 8) << " (" << YELLOWw(history.hodlingAmount*history.priceSell*0.999, 8) << 
					"),\tCurrent: " << YELLOWw(price, 8) << " (" << YELLOWw(history.hodlingAmount*price, 8) << ")" <<
					endl << endl;

			sumBoughtPrice += history.hodlingAmount*history.priceBuy*1.001;
			sumSoldPrice += history.hodlingAmount*history.priceSell*0.999;
			sumCurrentPrice += history.hodlingAmount*price*0.999;
		}
	}
	cout << "\nSummary:\n";
	cout << "\tbought " << REDw(sumBoughtPrice, 8) << ",\t   sold " << GREENw(sumSoldPrice, 8) << ",\t";
	if (sumSoldPrice > sumBoughtPrice)
		cout << GREENw(sumSoldPrice - sumBoughtPrice, 8) << " ( " << GREENw((sumSoldPrice - sumBoughtPrice)/sumBoughtPrice*100, 8) << GREEN(" %") << " )\n";
	else
		cout << REDw(sumSoldPrice - sumBoughtPrice, 8) << " ( " << REDw((sumSoldPrice - sumBoughtPrice)/sumBoughtPrice*100, 8) << RED(" %") << " )\n";	

	cout << "\tbought " << REDw(sumBoughtPrice, 8) << ",\tcurrent " << GREENw(sumCurrentPrice, 8) << ",\t";
	if (sumCurrentPrice > sumBoughtPrice)
		cout << GREENw(sumCurrentPrice - sumBoughtPrice, 8) << " ( " << GREENw((sumCurrentPrice - sumBoughtPrice)/sumBoughtPrice*100, 8) << GREEN(" %") << " )\n";
	else
		cout << REDw(sumCurrentPrice - sumBoughtPrice, 8) << " ( " << REDw((sumCurrentPrice - sumBoughtPrice)/sumBoughtPrice*100, 8) << RED(" %") << " )\n";	

	cout << "\t  sold " << REDw(sumSoldPrice, 8) << ",\tcurrent " << GREENw(sumCurrentPrice, 8) << ",\t";
	if (sumSoldPrice > sumCurrentPrice)
		cout << GREENw(sumSoldPrice - sumCurrentPrice, 8) << " ( " << GREENw((sumSoldPrice - sumCurrentPrice)/sumSoldPrice*100, 8) << GREEN(" %") << " )\n";
	else
		cout << REDw(sumSoldPrice - sumCurrentPrice, 8) << " ( " << REDw((sumSoldPrice - sumCurrentPrice)/sumSoldPrice*100, 8) << RED(" %") << " )\n";	


	cout << endl << endl;
	map <string, prices> exchanges;
	map <string, prices> coins;
	for (list<HodlingParams>::iterator iter = historyList.begin(); iter != historyList.end(); iter ++) {
		exchanges[iter->storedIn].buy = 0;
		exchanges[iter->storedIn].sell = 0;
		exchanges[iter->storedIn].diff = 0;
		coins[iter->symbol].buy = 0;
		coins[iter->symbol].sell = 0;
		coins[iter->symbol].diff = 0;
	}
	for (list<HodlingParams>::iterator iter = historyList.begin(); iter != historyList.end(); iter ++) { 
		exchanges[iter->storedIn].buy += (iter->priceBuy*iter->hodlingAmount)*1.001;
		exchanges[iter->storedIn].sell += (iter->priceSell*iter->hodlingAmount)*0.999;
		exchanges[iter->storedIn].diff = exchanges[iter->storedIn].sell - exchanges[iter->storedIn].buy;
		coins[iter->symbol].buy += (iter->priceBuy*iter->hodlingAmount)*1.001;
		coins[iter->symbol].sell += (iter->priceSell*iter->hodlingAmount)*0.999;
		coins[iter->symbol].diff = coins[iter->symbol].sell - coins[iter->symbol].buy;
	}
	
	vector <pair<string, prices>> coinsSorted;
	sortMapBasedOnPriceDiff(coins, coinsSorted);
	vector <pair<string, prices>> exchangesSorted;
	sortMapBasedOnPriceDiff(exchanges, exchangesSorted);

	for (vector <pair<string, prices>>::iterator iter = exchangesSorted.begin(); iter != exchangesSorted.end(); iter ++) {
		cout << "Exchange: " << YELLOWw(iter->first, 7) << ",  bought: " << REDw(iter->second.buy, 8) << ",  sold: " << GREENw(iter->second.sell, 8) << ",  ";
		if (iter->second.sell > iter->second.buy)
			cout << GREENw(iter->second.diff, 8) << " ( " << GREEN(iter->second.diff/iter->second.buy*100) << GREEN(" %") << " )\n";
		else
			cout << REDw(iter->second.diff, 8) << " ( " << RED(iter->second.diff/iter->second.buy*100) << RED(" %") << " )\n";
	}
	cout << endl;
	for (vector<pair<string, prices>>::iterator iter = coinsSorted.begin(); iter != coinsSorted.end(); iter ++) {
		cout << "Symbol: " << YELLOWw(iter->first, 4) << ",  bought: " << REDw(iter->second.buy, 8) <<  ",  sold: " << GREENw(iter->second.sell, 8) << ",  ";
		if (iter->second.sell > iter->second.buy)
			cout << GREENw(iter->second.diff, 10) << " ( " << GREEN(iter->second.diff/iter->second.buy*100) << GREEN(" %") << " )\n";
		else
			cout << REDw(iter->second.diff, 10) << " ( " << RED(iter->second.diff/iter->second.buy*100) << RED(" %") << " )\n";
	}
}

void BotMethod::ShowBalanceInUSDT() {
	map <string, StructBalanceInUSDT> balanceAllExchanges;

	map <string, StructBalanceInUSDT> balanceInUSDTBinance = binance->ShowBalanceInUSDT();
	map <string, StructBalanceInUSDT> balanceInUSDTCoinex = coinex->ShowBalanceInUSDT();
	map <string, StructBalanceInUSDT> balanceInUSDTHitBTC = hitbtc->ShowBalanceInUSDT();
	map <string, StructBalanceInUSDT> balanceInUSDTKucoin = kucoin->ShowBalanceInUSDT();

	for (map <string, StructBalanceInUSDT>::iterator iter = balanceInUSDTBinance.begin(); iter != balanceInUSDTBinance.end(); iter++) {
		balanceAllExchanges[iter->first].balance += iter->second.balance;
		balanceAllExchanges[iter->first].balanceFree += iter->second.balanceFree;
		balanceAllExchanges[iter->first].balanceLocked += iter->second.balanceLocked;
		balanceAllExchanges[iter->first].asset += iter->second.asset;
	}
	for (map <string, StructBalanceInUSDT>::iterator iter = balanceInUSDTCoinex.begin(); iter != balanceInUSDTCoinex.end(); iter++) {
		balanceAllExchanges[iter->first].balance += iter->second.balance;
		balanceAllExchanges[iter->first].balanceFree += iter->second.balanceFree;
		balanceAllExchanges[iter->first].balanceLocked += iter->second.balanceLocked;
		balanceAllExchanges[iter->first].asset += iter->second.asset;
	}
	for (map <string, StructBalanceInUSDT>::iterator iter = balanceInUSDTHitBTC.begin(); iter != balanceInUSDTHitBTC.end(); iter++) {
		balanceAllExchanges[iter->first].balance += iter->second.balance;
		balanceAllExchanges[iter->first].balanceFree += iter->second.balanceFree;
		balanceAllExchanges[iter->first].balanceLocked += iter->second.balanceLocked;
		balanceAllExchanges[iter->first].asset += iter->second.asset;
	}
	for (map <string, StructBalanceInUSDT>::iterator iter = balanceInUSDTKucoin.begin(); iter != balanceInUSDTKucoin.end(); iter++) {
		balanceAllExchanges[iter->first].balance += iter->second.balance;
		balanceAllExchanges[iter->first].balanceFree += iter->second.balanceFree;
		balanceAllExchanges[iter->first].balanceLocked += iter->second.balanceLocked;
		balanceAllExchanges[iter->first].asset += iter->second.asset;
	}

	double wholeBalance = 0;
	for (map <string, StructBalanceInUSDT>::iterator iter = balanceAllExchanges.begin(); iter != balanceAllExchanges.end(); iter++)
		wholeBalance += iter->second.asset;

	vector <pair<string, StructBalanceInUSDT>> balanceAllExchangesSorted;
	sortMapBasedOnAsset(balanceAllExchanges, balanceAllExchangesSorted);

	for (vector <pair<string, StructBalanceInUSDT>>::iterator iter = balanceAllExchangesSorted.begin(); iter != balanceAllExchangesSorted.end(); iter ++) {
		cout << iter->first << ": " << YELLOW(iter->second.balance) << " (" << iter->second.balanceFree << " , " 
			<< iter->second.balanceLocked << ") with asset value " << RED(iter->second.asset) << " (" 
			<< YELLOW(iter->second.asset/wholeBalance*100) << YELLOW(" %") << ")\n";
	}

	// for (map <string, StructBalanceInUSDT>::iterator iter = balanceAllExchanges.begin(); iter != balanceAllExchanges.end(); iter++) {
	// 	cout << iter->first << ": " << YELLOW(iter->second.balance) << " (" << iter->second.balanceFree << " , " 
	// 		<< iter->second.balanceLocked << ") with asset value " << RED(iter->second.asset) << " (" 
	// 		<< YELLOW(iter->second.asset/wholeBalance*100) << YELLOW(" %") << ")\n";
	// }

	cout << "\nAll exchanges wholeBalance is " << YELLOW(wholeBalance) << " USDT\n\n";
}

void BotMethod::ShowAllExchangesPrice(int mode) {
	int lenBinance = 0, lenRamzinex = 0, lenCoinex = 0, lenHitBTC = 0, lenKucoin = 0;
	SymbolPriceSrtuct priceBinance[200], priceRamzinex[200], priceCoinex[200], priceHitBTC[200], priceKucoin[200];

	string str;

	if (mode & BINANCE) {
		str = "WatchList";
		binance->GetPrices(str, priceBinance, lenBinance);
	}

	// if (mode & RAMZINEX) {
	// 	string str = "All";
	// 	Ramzinex::GetPrices(str, priceRamzinex, lenRamzinex);
	// }

	if (mode & COINEX) {
		str = "WatchList";
		coinex->GetPrices(str, priceCoinex, lenCoinex);
	}

	if (mode & HITBTC) {
		str = "WatchList";
		hitbtc->GetPrices(str, priceHitBTC, lenHitBTC);
	}

	if (mode & KUCOIN) {
		str = "WatchList";
		kucoin->GetPrices(str, priceKucoin, lenKucoin);
	}
	

	map <string, strcutExchangesPrices> mapExchanges;

	for (int i=0; i<lenBinance; i++)
		mapExchanges[priceBinance[i].symbol].binancePrice = priceBinance[i].price;
	for (int i=0; i<lenRamzinex; i++)
		mapExchanges[priceRamzinex[i].symbol].ramzinexPrice = priceRamzinex[i].price;
	for (int i=0; i<lenCoinex; i++)
		mapExchanges[priceCoinex[i].symbol].coinexPrice = priceCoinex[i].price;
	for (int i=0; i<lenHitBTC; i++) {
		if (priceHitBTC[i].symbol != "XRPUSDT")
			priceHitBTC[i].symbol += "T";
		mapExchanges[priceHitBTC[i].symbol].hitBTCPrice = priceHitBTC[i].price;
	}
	for (int i=0; i<lenKucoin; i++) {
		priceKucoin[i].symbol.erase (remove(priceKucoin[i].symbol.begin(), priceKucoin[i].symbol.end(), '-'), priceKucoin[i].symbol.end());
		mapExchanges[priceKucoin[i].symbol].kucoinPrice = priceKucoin[i].price;
	}

	cout << endl;
	cout << "\t" << setfill('-') << setw(121) << "" << setfill(' ') << endl;
	cout << "\t|" << setw(15) << "Symbol     |" << setw(15) << "Binance    |" << setw(15) << "Coinex    |" 
				<< setw(15) << "HitBTC    |" << setw(15) << "Kucoin    |" 
				<< setw(15) << "Minimum    |" << setw(15) << "Maximum    |" << setw(15) << "Percent    |" << "\n";
	cout << "\t" << setfill('-') << setw(121) << "" << setfill(' ') << endl;
	double min, max;
	for (map <string, strcutExchangesPrices>::iterator iter = mapExchanges.begin(); iter != mapExchanges.end(); iter++) {
		_findMinAndMaxFromStrcutExchangesPrices(iter->second, min, max);
		cout << "\t|" << setw(15) << iter->first+"    |"
					<< setw(15) << to_string(iter->second.binancePrice) + "  |" << setw(15) << to_string(iter->second.coinexPrice) + "  |"
					<< setw(15) << to_string(iter->second.hitBTCPrice) + "  |" << setw(15) << to_string(iter->second.kucoinPrice) + "  |"
					<< setw(15) << to_string(min) + "  |" << setw(15) << to_string(max) + "  |" 
					<< "  " << YELLOW(to_string((max-min)/min*100) + " %  ") << "|" << endl;
	}
	cout << "\t" << setfill('-') << setw(121) << "" << endl;
	cout << endl;
}

void BotMethod::Init() {
	binance = Binance::getInstance();
	coinex = Coinex::getInstance();
	hitbtc = HitBTC::getInstance();
	kucoin = Kucoin::getInstance();
}

void BotMethod::Cleanup() {
	// cout << "Successfully perform Menu cleaning up\n";
}
