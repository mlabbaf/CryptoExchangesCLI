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


using namespace std;


#define BINANCE_HOST "https://api.binance.com"


class Binance
{
	static CURL* curl;
	static Json::Value json_result;
	static Json::FastWriter fastWriter;
	static string secret_key;
	static string api_key;

	static int _pathQueryStringToUrl(string &url, string baseAddress, string symbol, string asset, 
									int limit, long fromId, long recvWindow, int status, long startTime, long endTime);
	static int _pathQueryStringToUrl_2(string &url, string baseAddress, string symbol, string side, string type, 
									 string timeInForce, double quantity, double price, long orderId, long recvWindow);

	static void _GetAllPrices();
	static void _GetPriceBySymbol(string &symbol);
	static void _GetAccountInfoBalances();
	static void _GetAllOrders(string &str, int PastDay);
	static void _GetMyTrades(string &str, int PastDay);

public:
	static void SetCurl(CURL* c) {
		curl = c;
	}
	static void Init();
	static void Cleanup();

	static void ShowServerTime();
	static void TestConnectivity();
	static void ShowAccountStatus();
	static void ShowExchangeInfo();
	static void GetPrices(string &str, SymbolPriceSrtuct* result, int &len);
	static void ShowPrices(string str);
	static map <string, map<string, double>> GetBalances();
	static void ShowBalances();
	// static void ShowBalanceInUSDT();
	static map <string, StructBalanceInUSDT> ShowBalanceInUSDT();
	static bool GetOpenOrders(string &str, Json::Value &jsonOpenOrders);
	static void ShowOpenOrders(string str);
	static void ShowAllOrders(string &str, int PastDay);
	static void ShowTradesPerformance(string &str, int PastDay);
	static void ShowMyTrades(string str, int PastDay);
	static void ShowDepositAddress(string &str);
	static void ShowDepositHistory(string str);
	static void ShowWithdrawHistory(string str);
	static void SendOrder(string symbol, string side, string type, double quantity, double price);
	static void CancelOrder(string symbol, long orderId);
};

#endif