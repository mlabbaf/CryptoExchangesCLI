#ifndef EXCHANGESHANDLER_H
#define EXCHANGESHANDLER_H

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


class Exchange
{
protected:
	string keyFilePath;
	string watchlistPath;
	string secret_key;
	string api_key;
	string access_id;
	string passphrase;
	string token;
	string exchangeName;

	CURL* curl;
	Json::Value json_result;

	virtual bool isJsonResultValid(Json::Value json_result) = 0;
	
	virtual void getAllPrices(string str, SymbolPriceSrtuct* result, int &len) = 0;
	virtual void getWatchlistPrices(string str, SymbolPriceSrtuct* result, int &len) = 0;
	virtual void getSymbolPrice(string str, SymbolPriceSrtuct* result, int &len) = 0;

public:
	void Init();
	void SetCurl(CURL* c) {
		curl = c;
	}
	void Cleanup();

	void setExchangeName(string name);

	void initApiSecret();
	void initApiSecretPassphrase();
	void initToken();

	virtual void ShowServerTime() = 0;
	void GetPrices(string &str, SymbolPriceSrtuct* result, int &len);
	void ShowPrices(string str);
	virtual map <string, map<string, double>> GetBalances(_GetBalancesModes mode) = 0;
	void ShowBalances();
	virtual map <string, StructBalanceInUSDT> ShowBalanceInUSDT() = 0;
	virtual bool GetOpenOrders(string &str, Json::Value &jsonOpenOrders) = 0;
	virtual bool GetOpenOrders(string &str, vector <SymbolOrderStruct> &vecOpenOrders) = 0; // Coinex used this protorype!
	virtual void SendOrder( string symbol, string side, string type, 
							double quantity, double price, double stopPrice, double stopLimitPrice) = 0;
	virtual void CancelOrder(string symbol, string orderId) = 0;
	virtual void CancelAllOrders(string symbol) = 0;
};

#endif