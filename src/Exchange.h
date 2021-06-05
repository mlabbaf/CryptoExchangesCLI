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
	string secret_key;
	string api_key;
	string access_id;
	string passphrase;

	CURL* curl;

public:
	void Init();
	void SetCurl(CURL* c) {
		curl = c;
	}
	void Cleanup();

	void test() {cout << "Hello from Exchange\n"; }
	
	void setKeyFilePath(string exchangeKeyFilePath);

	void InitApiSecret();
	void InitApiSecretPassphrase();

	virtual void GetPrices(string &str, SymbolPriceSrtuct* result, int &len) = 0;
	void ShowPrices(string str);
	virtual map <string, map<string, double>> GetBalances(_GetBalancesModes mode) = 0;
	void ShowBalances();
};

#endif