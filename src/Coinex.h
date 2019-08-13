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

class Coinex
{
	static CURL* curl;
	static Json::Value json_result;
	static Json::FastWriter fastWriter;
	static string secret_key;
	static string access_id;

	
	static int _pathQueryStringToUrl(	string &url, string &authorization, string baseAddress, string coin_type,
										string market, long id, long page, long limit, string type, double amount,
										double price);
	// static int _pathQueryStringToUrl_2(string &url, string baseAddress, string symbol, string side, string type, 
	// 								 string timeInForce, double quantity, double price, long orderId, long recvWindow);
	static int _createJsonData(string &postData, string market, string type, double amount, double price);
	
	static void _GetPriceBySymbol(string &symbol);
	static void _GetAccountInfoBalances();
	static bool _GetOpenOrdersForOneSymbol(string symbol, vector<SymbolOrderStruct> &vecOpenOrders);
	// static void _GetAllOrders(string &str, int PastDay);
	static void _GetMyTrades(string &str, int limit);



public:
	static void SetCurl(CURL* c) {
		curl = c;
	}
	static void Init();
	static void Cleanup();

	// static void ShowServerTime();
	// static void TestConnectivity();
	// static void ShowAccountStatus();
	// static void ShowExchangeInfo();
	static void GetPrices(string &str, SymbolPriceSrtuct* result, int &len);
	static void ShowPrices(string str);
	static map <string, map<string, double>> GetBalances();
	static void ShowBalances();
	static map <string, StructBalanceInUSDT> ShowBalanceInUSDT();
	static bool GetOpenOrders(string &str, vector <SymbolOrderStruct> &vecOpenOrders);
	static void ShowOpenOrders(string str);
	// static void ShowAllOrders(string &str, int PastDay);
	// static void ShowTradesPerformance(string &str, int PastDay);
	static void ShowMyTrades(string str, int pastDay);
	static void ShowTradesPerformance(string &str, int PastDay);
	// static void ShowDepositAddress(string &str);
	static void ShowDepositHistory(string str);
	static void ShowWithdrawHistory(string str);
	static void SendOrder(string symbol, string side, string type, double quantity, double price);
	static void CancelOrder(string symbol, long orderId);
};

#endif