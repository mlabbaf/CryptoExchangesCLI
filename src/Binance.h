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

	Json::FastWriter fastWriter;

	int _pathQueryStringToUrl(	string &url, string baseAddress, string symbol, string asset, string coin, string network,
								int limit, long fromId, long recvWindow, int status, long startTime, long endTime);
	int _pathQueryStringToUrl_2(	string &url, string baseAddress, string symbol, string side, string type, 
									string timeInForce, double quantity, double price, double ocoStopPrice,
									double stopLimitPrice, string orderId, long recvWindow);

	void _GetAccountInfoBalances();
	void _GetAllOrders(string &str, int PastDay);
	void _GetMyTrades(string &str, int PastDay);

protected:
	bool isJsonResultValid(Json::Value json_result);
	
	void getAllPrices(string str, SymbolPriceSrtuct* result, int &len);
	void getWatchlistPrices(string str, SymbolPriceSrtuct* result, int &len);
	void getSymbolPrice(string str, SymbolPriceSrtuct* result, int &len);

public:
	static Binance* getInstance();

	void ShowServerTime();
	void TestConnectivity();
	void ShowAccountStatus();
	void ShowExchangeInfo();
	map <string, map<string, double>> GetBalances(_GetBalancesModes mode);
	map <string, StructBalanceInUSDT> ShowBalanceInUSDT();
	bool GetOpenOrders(string &str, Json::Value &jsonOpenOrders);
	bool GetOpenOrders(string &str, vector <SymbolOrderStruct> &vecOpenOrders);
	void ShowOpenOrders(string str);
	void ShowAllOrders(string &str, int PastDay);
	void ShowTradesPerformance(string &str, int PastDay);
	void ShowMyTrades(string str, int PastDay);
	void ShowDepositAddress(string &coin, string &network);
	void ShowDepositHistory(string str);
	void ShowWithdrawHistory(string str);
	void SendOrder( string symbol, string side, string type, 
					double quantity, double price, double stopPrice, double stopLimitPrice);
	void CancelOrder(string symbol, string orderId);
	void CancelAllOrders(string symbol);

	// void test();
};

#endif