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


using namespace std;


#define KUCOIN_HOST "https://api.kucoin.com"


class Kucoin
{
private:
	static CURL* curl;
	static Json::Value json_result;
	static Json::FastWriter fastWriter;
	static string secret_key;
	static string api_key;
	static string passphrase;

	static void _FillKucoinFields(StructKucoinHttpFields &fields, string &endpoint, string method, string body);
	
	static int _createJsonData(	string &postData, string symbol, string currency, string side, string from, string to, 
								double size, double amount, double price, double stopPrice);
	static int _pathQueryStringToUrl(	string &url, string baseAddress, string &querystring, string status, string symbol,
										string currency, long startAt);
// 	static int _pathQueryStringToUrl_2(string &url, string baseAddress, string symbol, string side, string type, 
// 									 string timeInForce, double quantity, double price, long orderId, long recvWindow);

	static void _GetAllPrices();
	static void _GetPriceBySymbol(string &symbol);
	static void _GetAccountInfoBalances();
// 	static void _GetAllOrders(string &str, int PastDay);
	static void _GetMyTrades(string &str, int PastDay);

public:
	static void SetCurl(CURL* c) {
		curl = c;
	}
	static void Init();
	static void Cleanup();

	static void ShowServerTime();
// 	static void TestConnectivity();
// 	static void ShowAccountStatus();
// 	static void ShowExchangeInfo();
	static void GetPrices(string &str, SymbolPriceSrtuct* result, int &len);
	static void ShowPrices(string str);
	static map <string, map<string, double>> GetBalances(_GetBalancesModes mode);
	static void ShowBalances();
	static void ShowBankBalances();
	static void ShowExchangeBalances();
	static map <string, StructBalanceInUSDT> ShowBalanceInUSDT();
	static bool GetOpenOrders(string &str, Json::Value &jsonOpenOrders);
	static void ShowOpenOrders(string str);
// 	static void ShowAllOrders(string &str, int PastDay);
	static void ShowMyTrades(string str, int PastDay);
	static void ShowTradesPerformance(string &str, int PastDay);
	static void ShowDepositAddress(string &str);
	static void ShowDepositHistory(string str);
	static void ShowWithdrawHistory(string str);
	static void SendOrder(string symbol, string side, string type, double size, double price);
	static void CancelOrder(string symbol, string orderId);
	static void CancelAllOrders(string symbol);
	static void TransferBetweenBankAndExchange(string currency, double amount, string type);
};

#endif
