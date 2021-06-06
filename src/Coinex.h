#ifndef COINEX_H
#define COINEX_H

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
#include "Utility.h"
#include "Definitions.h"
#include "Exchange.h" 

using namespace std;


#define COINEX_HOST "https://api.coinex.com/v1"



class Coinex : public Exchange
{
private:
	static Coinex* instance;

	Json::FastWriter fastWriter;

	
	int _pathQueryStringToUrl(	string &url, string &authorization, string baseAddress, string coin_type,
										string market, string id, long page, long limit, string type, double amount,
										double price);
	// int _pathQueryStringToUrl_2(string &url, string baseAddress, string symbol, string side, string type, 
	// 								 string timeInForce, double quantity, double price, long orderId, long recvWindow);
	int _createJsonData(string &postData, string market, string type, double amount, double price);
	
	void _GetAccountInfoBalances();
	bool _GetOpenOrdersForOneSymbol(string symbol, vector<SymbolOrderStruct> &vecOpenOrders);
	// void _GetAllOrders(string &str, int PastDay);
	void _GetMyTrades(string &str, int limit);



protected:
	bool isJsonResultValid(Json::Value json_result);
	
	void getAllPrices(string str, SymbolPriceSrtuct* result, int &len);
	void getWatchlistPrices(string str, SymbolPriceSrtuct* result, int &len);
	void getSymbolPrice(string str, SymbolPriceSrtuct* result, int &len);

public:
	static Coinex* getInstance();

	void ShowServerTime();
	// void TestConnectivity();
	// void ShowAccountStatus();
	// void ShowExchangeInfo();
	map <string, map<string, double>> GetBalances(_GetBalancesModes mode);
	map <string, StructBalanceInUSDT> ShowBalanceInUSDT();
	bool GetOpenOrders(string &str, Json::Value &jsonOpenOrders);
	bool GetOpenOrders(string &str, vector <SymbolOrderStruct> &vecOpenOrders);
	void ShowOpenOrders(string str);
	// void ShowAllOrders(string &str, int PastDay);
	// void ShowTradesPerformance(string &str, int PastDay);
	void ShowMyTrades(string str, int pastDay);
	void ShowTradesPerformance(string &str, int PastDay);
	// void ShowDepositAddress(string &str);
	void ShowDepositHistory(string str);
	void ShowWithdrawHistory(string str);
	void SendOrder(	string symbol, string side, string type,
	 				double quantity, double price, double stopPrice, double stopLimitPrice);
	void CancelOrder(string symbol, string orderId);
	void CancelAllOrders(string symbol);
};

#endif