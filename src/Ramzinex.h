#ifndef RAMZINEX_H
#define RAMZINEX_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <exception>
#include <algorithm>
#include <fstream>
#include <unistd.h>

#include <curl/curl.h>
#include <json/json.h>


using namespace std;


#define RAMZINEX_HOST "https://ramzinex.com/exchange/api/exchange"


class Ramzinex
{
	static CURL* curl;
	static Json::Value json_result;
	static Json::FastWriter fastWriter;
	
	static void _GetAllPrices();

public:
	static void SetCurl(CURL* c) {
		curl = c;
	}
	static void Init();
	static void Cleanup();

	static Json::Value GetPrices(string &str);
	static void ShowPrices(string str);
};

#endif