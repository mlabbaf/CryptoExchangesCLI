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


struct SymbolOrderStruct {
	string symbol;
	double price;
	string type;
	double amount;
	double left;
	string order_type;
	long id;
};

class Coinex : public Exchange
{
private:
	static Coinex* instance;

	Json::Value json_result;
	Json::FastWriter fastWriter;

	
	int _pathQueryStringToUrl(	string &url, string &authorization, string baseAddress, string coin_type,
										string market, long id, long page, long limit, string type, double amount,
										double price);
	// int _pathQueryStringToUrl_2(string &url, string baseAddress, string symbol, string side, string type, 
	// 								 string timeInForce, double quantity, double price, long orderId, long recvWindow);
	int _createJsonData(string &postData, string market, string type, double amount, double price);
	
	void _GetPriceBySymbol(string &symbol);
	void _GetAccountInfoBalances();
	bool _GetOpenOrdersForOneSymbol(string symbol, vector<SymbolOrderStruct> &vecOpenOrders);
	// void _GetAllOrders(string &str, int PastDay);
	void _GetMyTrades(string &str, int limit);



public:
	static Coinex* getInstance();

	// void ShowServerTime();
	// void TestConnectivity();
	// void ShowAccountStatus();
	// void ShowExchangeInfo();
	void GetPrices(string &str, SymbolPriceSrtuct* result, int &len);
	void ShowPrices(string str);
	map <string, map<string, double>> GetBalances(_GetBalancesModes mode);
	void ShowBalances();
	map <string, StructBalanceInUSDT> ShowBalanceInUSDT();
	bool GetOpenOrders(string &str, vector <SymbolOrderStruct> &vecOpenOrders);
	void ShowOpenOrders(string str);
	// void ShowAllOrders(string &str, int PastDay);
	// void ShowTradesPerformance(string &str, int PastDay);
	void ShowMyTrades(string str, int pastDay);
	void ShowTradesPerformance(string &str, int PastDay);
	// void ShowDepositAddress(string &str);
	void ShowDepositHistory(string str);
	void ShowWithdrawHistory(string str);
	void SendOrder(string symbol, string side, string type, double quantity, double price);
	void CancelOrder(string symbol, long orderId);
};

#endif