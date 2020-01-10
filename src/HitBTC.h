#ifndef HITBTC_H
#define HITBTC_H

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


#define HITBTC_HOST "https://api.hitbtc.com/api/2"



class HitBTC : public Exchange
{
private:
	static HitBTC* instance;

	CURL* curl;
	Json::Value json_result;
	Json::FastWriter fastWriter;

	int _pathQueryStringToUrl(	string &url, string baseAddress, string symbol, string from, string until,
										string side, string type, string clientOrderId, string by);
	int _createJsonData(	string &postData, string symbol, string currency, string side, 
								string type, double quantity, double amount, double price, double stopPrice);
	void _GetAllPrices();
	void _GetPriceBySymbol(string &symbol);
	void _GetAccountInfoBalances();
	void _GetTradingBalances();
// 	void _GetAllOrders(string &str, int PastDay);
	void _GetMyTrades(string &str, int PastDay);

public:
	static HitBTC* getInstance();

	void test() { cout << "HitBTC test\n"; }
	void SetCurl(CURL* c) {
		curl = c;
	}
	void Init();
	void Cleanup();

// 	void ShowServerTime();
// 	void TestConnectivity();
// 	void ShowAccountStatus();
// 	void ShowExchangeInfo();
	void GetPrices(string &str, SymbolPriceSrtuct* result, int &len);
	void ShowPrices(string str);
	map <string, map<string, double>> GetBalances(_GetBalancesModes mode);
	void ShowBalances();
	void ShowBankBalances();
	void ShowExchangeBalances();
	map <string, StructBalanceInUSDT> ShowBalanceInUSDT();
	bool GetOpenOrders(string &str, Json::Value &jsonOpenOrders);
	void ShowOpenOrders(string str);
// 	void ShowAllOrders(string &str, int PastDay);
// 	void ShowTradesPerformance(string &str, int PastDay);
	void ShowMyTrades(string str, int PastDay);
	void ShowTradesPerformance(string &str, int PastDay);
	void ShowDepositAddress(string &str);
// 	void ShowDepositHistory(string str);
// 	void ShowWithdrawHistory(string str);
	void SendOrder(string symbol, string side, string type, double quantity, double price);
	void CancelOrder(string symbol, string clientOrderId);
	void TransferBetweenBankAndExchange(string currency, double amount, string type);
};

#endif