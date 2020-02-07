#ifndef CURLAPI_H
#define CURLAPI_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <exception>
#include <thread>
#include <chrono>
#include <future>
#include <iomanip>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include "Utility.h"
#include "Definitions.h"

#include <curl/curl.h>
#include <json/json.h>



using namespace std;


// #define CHECK_IP_ADDRESS_HOST	"http://checkip.dyndns.org/"
#define GET_IP_ADDRESS_HOST 			"https://myexternalip.com/raw"
// #define GET_LOCATION_FRON_IP_ADDRESS 	"http://api.db-ip.com/v2/free/"
#define GET_LOCATION_FRON_IP_ADDRESS	"http://ip-api.com/json/"


class CurlAPI
{
	static CURL* curl;
	static int progressCnt;
	static time_t timeStart, timeEnd;

	static void _waiting(future<void> futureObj);

	static size_t curl_cb( void *content, size_t size, size_t nmemb, std::string *buffer);
	static int curl_cb_debug(CURL *handle, curl_infotype type, char *data, size_t size, void *userp);
	static void curl_api_with_header(string &url, string &str_result, vector <string> &extra_http_header, string &post_data, string &action, string user_pwd);

public:
	static void CurlInit();
	static void CurlCleanup();
	static CURL* GetCurl() {
		return curl;
	}
	static void ResetProgressCnt() {
		progressCnt = 0;
		time (&timeStart);
	}


	// General
	static void CurlGetRequest(string &url, string &str_result);
	
	// Binance
	static void CurlGetRequestWithMBXKey(string &url, string &str_result, string &api_key);
	static void CurlPostRequestWithMBXKey(string &url, string &str_result, string &api_key);
	static void CurlDeleteRequestWithMBXKey(string &url, string &str_result, string &api_key);
	
	// Coinex
	static void CurlGetRequestWithAuthorization(string &url, string &str_result, string &authorization);
	static void CurlPostRequestWithAuthorization(string &url, string &str_result, string &authorization, string &post_data);
	static void CurlDeleteRequestWithAuthorization(string &url, string &str_result, string &authorization);
	
	// HitBTC
	static void CurlGetRequestWithBasicAuthentication(string &url, string &str_result, string &api_key, string &secret_key);
	static void CurlPostRequestWithBasicAuthentication(string &url, string &str_result, string &api_key, string &secret_key, string &post_data);
	static void CurlDeleteRequestWithBasicAuthentication(string &url, string &str_result, string &api_key, string &secret_key);

	// Kucoin
	static void CurlGetRequestWithKCKey(string &url, string &str_result, StructKucoinHttpFields &fields);
	static void CurlPostRequestWithKCKey(string &url, string &str_result, StructKucoinHttpFields &fields, string &post_data);
	static void CurlDeleteRequestWithKCKey(string &url, string &str_result, StructKucoinHttpFields &fields);
	
	static void ShowExternalIpAddress();
};


#endif