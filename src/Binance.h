#ifndef BINANCE_H
#define BINANCE_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <exception>
#include <fstream>
#include <algorithm>
#include <sys/time.h>

#include <curl/curl.h>
#include <json/json.h>

#include "Log.h"
#include "Definitions.h"
#include "Exchange.h" 


using namespace std;


#define BINANCE_HOST "https://api.binance.com"


class Binance : public Exchange
{
private:
	static Binance* instance;

	Json::Value json_result;
	Json::FastWriter fastWriter;

	int _pathQueryStringToUrl(string &url, string baseAddress, string symbol, string asset, string coin, string network,
									int limit, long fromId, long recvWindow, int status, long startTime, long endTime);
	int _pathQueryStringToUrl_2(string &url, string baseAddress, string symbol, string side, string type, 
									 string timeInForce, double quantity, double price, double ocoStopPrice,
									 double stopLimitPrice, long orderId, long recvWindow);

	void _GetAllPrices();
	void _GetPriceBySymbol(string &symbol);
	void _GetAccountInfoBalances();
	void _GetAllOrders(string &str, int PastDay);
	void _GetMyTrades(string &str, int PastDay);

public:
	static Binance* getInstance();

	void ShowServerTime();
	void TestConnectivity();
	void ShowAccountStatus();
	void ShowExchangeInfo();
	void GetPrices(string &str, SymbolPriceSrtuct* result, int &len);
	void ShowPrices(string str);
	map <string, map<string, double>> GetBalances(_GetBalancesModes mode);
	void ShowBalances();
	map <string, StructBalanceInUSDT> ShowBalanceInUSDT();
	bool GetOpenOrders(string &str, Json::Value &jsonOpenOrders);
	void ShowOpenOrders(string str);
	void ShowAllOrders(string &str, int PastDay);
	void ShowTradesPerformance(string &str, int PastDay);
	void ShowMyTrades(string str, int PastDay);
	void ShowDepositAddress(string &coin, string &network);
	void ShowDepositHistory(string str);
	void ShowWithdrawHistory(string str);
	void SendOrder(string symbol, string side, string type, double quantity, 
						  double price, double stopPrice, double stopLimitPrice);
	void CancelOrder(string symbol, long orderId);

	// void test();
};

#endif