#ifndef KUCOIN_H
#define KUCOIN_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <exception>
#include <fstream>
#include <algorithm>
#include <sys/time.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>

#include <curl/curl.h>
#include <json/json.h>

#include "Log.h"
#include "Definitions.h"
#include "Utility.h"
#include "Exchange.h" 

using namespace std;


#define KUCOIN_HOST "https://api.kucoin.com"


class Kucoin : public Exchange
{
private:
	static Kucoin* instance;

	CURL* curl;
	Json::Value json_result;
	Json::FastWriter fastWriter;

	void _FillKucoinFields(StructKucoinHttpFields &fields, string &endpoint, string method, string body);
	
	int _createJsonData(	string &postData, string symbol, string currency, string side, string from, string to, 
								double size, double amount, double price, double stopPrice);
	int _pathQueryStringToUrl(	string &url, string baseAddress, string &querystring, string status, string symbol,
										string currency, long startAt);
// 	int _pathQueryStringToUrl_2(string &url, string baseAddress, string symbol, string side, string type, 
// 									 string timeInForce, double quantity, double price, long orderId, long recvWindow);

	void _GetAllPrices();
	void _GetPriceBySymbol(string &symbol);
	void _GetAccountInfoBalances();
// 	void _GetAllOrders(string &str, int PastDay);
	void _GetMyTrades(string &str, int PastDay);

public:
	static Kucoin* getInstance();

	void SetCurl(CURL* c) {
		curl = c;
	}
	void Init();
	void Cleanup();

	void ShowServerTime();
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
	void ShowMyTrades(string str, int PastDay);
	void ShowTradesPerformance(string &str, int PastDay);
	void ShowDepositAddress(string &str);
	void ShowDepositHistory(string str);
	void ShowWithdrawHistory(string str);
	void SendOrder(string symbol, string side, string type, double size, double price);
	void CancelOrder(string symbol, string orderId);
	void CancelAllOrders(string symbol);
	void TransferBetweenBankAndExchange(string currency, double amount, string type);
};

#endif
