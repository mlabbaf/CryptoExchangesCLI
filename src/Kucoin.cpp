#include "Kucoin.h"
#include "CurlAPI.h"
#include "binacpp_utils.h"


using namespace std;

Kucoin* Kucoin::instance = 0;
Kucoin* Kucoin::getInstance() {
    if (instance == 0) {
        instance = new Kucoin();
    }
    return instance;
}


static bool _IsJsonResultValid(Json::Value json_result) {
	// if (json_result.isObject()) {
		if (Utility::JsonToLong(json_result["code"]) == 200000)
			return true;
		else {
			cout << json_result << endl;
			cout << RED ("Invalid code in json_result\n");
			return false;
		}
	// }
	// else {
	// 	for (int i=0; i<json_result.size(); i++)
	// 		if (json_result[i]["code"].asInt64() != 0) {
	// 			cout << json_result[i] << endl;
	// 			cout << RED ("Invalid code in json_result[" << RED (i) << "]");
	// 			return false;
	// 		}
	// 	return true;
	// }

	return true;
} 


static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

static string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
    	char_array_3[i++] = *(bytes_to_encode++);
    	if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i) {
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
  			ret += '=';
  	}

  	return ret;
}

static string _createAuthorizationSha256Encoding64( const char *key, const char *data) {
    unsigned char* digest;
    digest = HMAC(EVP_sha256(), key, strlen(key), (unsigned char*)data, strlen(data), NULL, NULL);    
    return base64_encode(digest, strlen((char*)digest));
} 

void Kucoin::_FillKucoinFields(StructKucoinHttpFields &fields, string &endpoint, string method, string body) {
	fields.apiKey = api_key;
	fields.passphrase = passphrase;
	fields.timestamp = to_string(get_current_ms_epoch());
	string what = fields.timestamp + method + endpoint + body;
	fields.signature = _createAuthorizationSha256Encoding64(secret_key.c_str(), what.c_str());
	
	// cout << "passphrase: " << fields.passphrase << endl;
	// cout << "api_key: " << fields.apiKey << endl;
	// cout << "timestamp: " << fields.timestamp << endl;
	// cout << "what: " << what << endl;
	// cout << "signature: " << fields.signature << endl;
	// cout << "secret_key: " << secret_key << endl;
}

int Kucoin::_createJsonData(string &postData, string symbol, string currency, string side, string from, string to, 
							double size, double amount, double price, double stopPrice) {

	postData = "{";
	
	if (symbol.size() != 0) {
		postData.append("\"symbol\":");
		postData.append("\"" + symbol + "\",");
	}

	if (currency.size() != 0) {
		postData.append("\"currency\":");
		postData.append("\"" + currency + "\",");
	}

	if (side.size() != 0) {
		transform(side.begin(), side.end(), side.begin(), ::tolower);
		postData.append("\"side\":");
		postData.append("\"" + side + "\",");
	}

	if (from.size() != 0) {
		postData.append("\"from\":");
		postData.append("\"" + from + "\",");
	}

	if (to.size() != 0) {
		postData.append("\"to\":");
		postData.append("\"" + to + "\",");
	}

	if (size > 0) {
		postData.append("\"size\":");
		postData.append("\"" + to_string(size) + "\",");
	}

	if (amount > 0) {
		postData.append("\"amount\":");
		postData.append("\"" + to_string(amount) + "\",");
	}

	if (price > 0) {
		postData.append("\"price\":");
		postData.append("\"" + to_string(price) + "\",");
	}

	if (stopPrice > 0) {
		postData.append("\"stopPrice\":");
		postData.append("\"" + to_string(stopPrice) + "\",");
	}

	postData.pop_back();
	// postData.pop_back();
	postData.append("}");

	cout << "postData: " << postData << endl;
	return 0;
}

int Kucoin::_pathQueryStringToUrl(	string &url, string baseAddress, string &querystring, string status, string symbol,
									string currency, long startTime) {
	url = KUCOIN_HOST;
	querystring = baseAddress;

	// string querystring(""); 

	if (status.size() != 0) {
		querystring.append("&status=");
		querystring.append(status);
	}

	if (symbol.size() != 0) {
		querystring.append("&symbol=");
		querystring.append(symbol);
	}

	if (currency.size() != 0) {
		querystring.append("&currency=");
		querystring.append(currency);
	}

	if ( startTime > 0 ) {
		querystring.append("&startAt=");
		querystring.append( to_string( startTime ) );
	}

	url.append(querystring);

	// cout << "url: " << url << endl;
	return 0;
}

static void _GetStartTimeFromDay(int PastDay, long &StartTime) {
	if (PastDay == 0) 	// All times
		StartTime = 0;
	else {
		struct timeval tv;

		gettimeofday(&tv, NULL); 

		StartTime = (tv.tv_sec*1000+tv.tv_usec/1000) - PastDay*24*60*60*1000;
		// cout << "StartTime: " << StartTime << ", PastDay: " << PastDay << endl;
	}
}

void Kucoin::Init() {
	Exchange::setKeyFilePath("config/KucoinKeys.txt");
	Exchange::InitApiSecretPassphrase();
}

void Kucoin::Cleanup() {
	Exchange::Cleanup();
	// cout << "Successfully perform Kucoin cleaning up\n";
}

void Kucoin::ShowServerTime() {
	// cout << "Kucoin::ShowServerTime\n";
	string url(KUCOIN_HOST);  
	url += "/api/v1/timestamp";

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside ShowServerTime, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Kucoin::ShowServerTime", str_result, json_result);

	if (_IsJsonResultValid(json_result)) 
		// cout << json_result << endl;
		cout << fastWriter.write(json_result) << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Kucoin::_GetAllPrices() {
	string url(KUCOIN_HOST);  
	url += "/api/v1/market/allTickers";

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside _GetAllPrices, str_result: " << str_result << endl;

	Utility::ParseStringToJson("_GetAllPrices", str_result, json_result);
}

void Kucoin::_GetPriceBySymbol(string &symbol) {
	// cout << symbol << " - _GetPriceBySymbol\n";
	string url(KUCOIN_HOST);  
	url += "/api/v1/market/orderbook/level1?symbol="+symbol;

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside _GetPriceBySymbol, str_result: " << str_result << endl;

	Utility::ParseStringToJson("_GetPriceBySymbol", str_result, json_result);
}

void Kucoin::GetPrices(string &str, SymbolPriceSrtuct* result, int &len) {

	if (str == "All") {
		Kucoin::_GetAllPrices();
		if (_IsJsonResultValid(json_result)) {
			int cnt = 0;
			Json::Value json_ticker = json_result["data"]["ticker"];
			for (Json::Value::const_iterator iter = json_ticker.begin(); iter != json_ticker.end(); iter++) {
				result[cnt].symbol = (*iter)["symbol"].asString();
				result[cnt].price = Utility::JsonToDouble((*iter)["last"]);
				cnt++;
			}
			len = cnt;
		}
	}
	else if (str == "WatchList") {
		Kucoin::_GetAllPrices();
		if (_IsJsonResultValid(json_result)) {
			string line;
	  		ifstream myfile ("config/WatchlistKucoin.txt");
	  		if (myfile.is_open()) {
	    		int cnt = 0;
	    		while ( getline (myfile, line) ) {
					Json::Value json_ticker = json_result["data"]["ticker"];
					for (Json::Value::const_iterator iter = json_ticker.begin(); iter != json_ticker.end(); iter++)
						if ((*iter)["symbol"] == line) {
							result[cnt].symbol = line;
							result[cnt].price = Utility::JsonToDouble((*iter)["last"]);
							cnt++;
						}
				}
	    		len = cnt;
	    		myfile.close();
	    	}
		}
		else {
			cout << "Cannot _GetAllPrices\n";
			len = 0;
			return;
		}
	}
	else {
		Kucoin::_GetPriceBySymbol(str);
		if (_IsJsonResultValid(json_result)) {
			result[0].symbol = str;
			result[0].price = Utility::JsonToDouble(json_result["data"]["price"]);
			len = 1;
		}
	} 
}

void Kucoin::ShowPrices(string str) {
	Exchange::ShowPrices(str);
}

void Kucoin::_GetAccountInfoBalances() {
	string url(KUCOIN_HOST);  
	string endpoint = "/api/v1/accounts";
	url += endpoint;
	
	string temp;
	StructKucoinHttpFields fields;
	_FillKucoinFields(fields, endpoint, "GET", temp);

	string str_result;
	CurlAPI::CurlGetRequestWithKCKey(url, str_result, fields);
	// cout << "inside _GetAccountInfoBalances, str_result: " << str_result << endl;

	Utility::ParseStringToJson("_GetAccountInfoBalances", str_result, json_result);

	if (_IsJsonResultValid(json_result)) {}
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

map <string, map<string, double>> Kucoin::GetBalances(_GetBalancesModes mode) {
	// cout << "GetBalances\n";

	map < string, map <string,double> >  userBalance;
	Kucoin::_GetAccountInfoBalances();
	Json::Value json_data = json_result["data"];
	// cout << json_data << endl;
	
	if (mode == BANK_AND_EXCHANGE_MODE || mode == ONLY_BANK_MODE) {
		if (_IsJsonResultValid(json_result)) {
			for (Json::Value::const_iterator json_iter = json_data.begin(); json_iter != json_data.end(); json_iter++) {
				if ((*json_iter)["type"].asString() == "main") {
					string symbol = (*json_iter)["currency"].asString();
					userBalance[symbol]["f"] += Utility::JsonToDouble((*json_iter)["available"]);
					userBalance[symbol]["l"] += Utility::JsonToDouble((*json_iter)["holds"]);
				}
			}
		}
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
		}
	}

	if (mode == BANK_AND_EXCHANGE_MODE || mode == ONLY_EXCHANGE_MODE) {
		if (_IsJsonResultValid(json_result)) {
			for (Json::Value::const_iterator json_iter = json_data.begin(); json_iter != json_data.end(); json_iter++) {
				if ((*json_iter)["type"].asString() == "trade") {
					string symbol = (*json_iter)["currency"].asString();
					userBalance[symbol]["f"] += Utility::JsonToDouble((*json_iter)["available"]);
					userBalance[symbol]["l"] += Utility::JsonToDouble((*json_iter)["holds"]);
				}
			}
		}
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
		}
	}

	return userBalance;
}

void Kucoin::ShowBalances() {
	Exchange::ShowBalances();
}

void Kucoin::ShowBankBalances() {
	// cout << "ShowBankBalances\n";
	
	map <string, map<string,double>> userBalance;
	userBalance = GetBalances(ONLY_BANK_MODE);

	cout << "==================================" << endl;
	
	map < string, map<string,double> >::iterator it_i;
	for ( it_i = userBalance.begin() ; it_i != userBalance.end() ; it_i++ ) {

		string symbol 			= (*it_i).first;
		map <string,double> balance 	= (*it_i).second;

		if (balance["f"] != 0 || balance["l"] != 0) {
			cout << "Symbol :" << symbol << ", \t";
			printf("Free   : %.08f, ", balance["f"] );
			printf("Locked : %.08f " , balance["l"] );
			cout << " " << endl;
		}
	}
}

void Kucoin::ShowExchangeBalances() {
	// cout << "ShowExchangeBalances\n";
	
	map <string, map<string,double>> userBalance;
	userBalance = GetBalances(ONLY_EXCHANGE_MODE);

	cout << "==================================" << endl;
	
	map < string, map<string,double> >::iterator it_i;
	for ( it_i = userBalance.begin() ; it_i != userBalance.end() ; it_i++ ) {

		string symbol 			= (*it_i).first;
		map <string,double> balance 	= (*it_i).second;

		if (balance["f"] != 0 || balance["l"] != 0) {
			cout << "Symbol :" << symbol << ", \t";
			printf("Free   : %.08f, ", balance["f"] );
			printf("Locked : %.08f " , balance["l"] );
			cout << " " << endl;
		}
	}
}

map <string, StructBalanceInUSDT> Kucoin::ShowBalanceInUSDT() {
	map <string, StructBalanceInUSDT> balanceResult;

	// Get balance
	map <string, map<string,double>> userBalance;
	userBalance = GetBalances(BANK_AND_EXCHANGE_MODE);

	// Get current prices
	string str = "All";
	SymbolPriceSrtuct result[2000];
	int len;
	Kucoin::GetPrices(str, result, len);


	// Calculate whole balances
	double wholeBalance = 0;
	for (map <string, map<string, double>>::iterator it_i=userBalance.begin(); it_i!= userBalance.end(); it_i++) {
		string symbol 			= (*it_i).first;
		map <string,double> balance 	= (*it_i).second;
		double coinBalance = balance["f"] + balance["l"];

		if (coinBalance > 0) {
			// Get price
			double price = -1;
			if (symbol == "USDT") 
				price = 1;
			else
				for (int i=0; i<len; i++) {
					if (result[i].symbol == (symbol+"-USDT"))
						price = result[i].price;
				}
			if (price == -1) {
				cout << YELLOW("Warning:") << " cannot find price for " << symbol << endl;
				price = 0;
				// return;
			}

			wholeBalance += coinBalance * price;
			cout << symbol << ": " << YELLOW(coinBalance) << " (" << balance["f"] << " , " << balance["l"] <<
				 ") at price " << YELLOW(price) << " (" << RED(coinBalance*price) << ")\n";
		
			balanceResult[symbol].balance = coinBalance;
			balanceResult[symbol].balanceFree = balance["f"];
			balanceResult[symbol].balanceLocked = balance["l"];
			balanceResult[symbol].price = price;
			balanceResult[symbol].asset = coinBalance*price;
		}
	}

	cout << "\nwholeBalance is " << YELLOW(wholeBalance) << " USDT\n\n";

	return balanceResult;
}

bool Kucoin::GetOpenOrders(string &str, Json::Value &jsonOpenOrders) {
	string url, querystring;
	string endpoint = "/api/v1/orders?";
	
	string temp;
	if (str == "All")
		_pathQueryStringToUrl(url, endpoint, querystring, "active", temp, temp, 0); 
	else
		_pathQueryStringToUrl(url, endpoint, querystring,"active", str, temp, 0); 
	// cout << "Inside GetOpenOrders, url: " << url << endl;
	
	StructKucoinHttpFields fields;
	_FillKucoinFields(fields, querystring, "GET", temp);

	string str_result;
	CurlAPI::CurlGetRequestWithKCKey(url, str_result, fields);
	// cout << "inside GetOpenOrders, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Kucoin::GetOpenOrders", str_result, jsonOpenOrders);

	if (_IsJsonResultValid(jsonOpenOrders)) 
		return true;
	else {
		cout << "jsonOpenOrders is not valid\n";
		cout << jsonOpenOrders << endl;
		return false;
	}
}

void Kucoin::ShowOpenOrders(string str) {
	// cout << "ShowOpenOrders\n";
	
	if (Kucoin::GetOpenOrders(str, json_result)) {
		Json::Value json_items = json_result["data"]["items"];
		for (Json::Value::const_iterator iter = json_items.begin(); iter != json_items.end(); iter++)
			cout << "symbol: " << YELLOW((*iter)["symbol"]) << ", price: " << YELLOW((*iter)["price"]) << 
					", side: " << (*iter)["side"] <<	", size: " << (*iter)["size"] << ", dealSize: " << (*iter)["dealSize"] <<
					", type: " << (*iter)["type"] << ", id: " << (*iter)["id"] << endl;
	}
	else
		cout << "Cannot GetOpenOrders\n";
}

void Kucoin::_GetMyTrades(string &str, int PastDay) {
	string url, querystring;
	string endpoint = "/api/v1/fills";
	if (str != "All" || PastDay != 0)
		endpoint += "?";
	
	long startTime;
	_GetStartTimeFromDay(PastDay, startTime);
	
	string temp;
	if (str == "All")
		_pathQueryStringToUrl(url, endpoint, querystring, temp, temp, temp, startTime);
	else 
		_pathQueryStringToUrl(url, endpoint, querystring, temp, str, temp, startTime);
	// cout << "inside _GetMyTrades, url: " << url << endl;
	
	StructKucoinHttpFields fields;
	_FillKucoinFields(fields, querystring, "GET", temp);

	string str_result;
	CurlAPI::CurlGetRequestWithKCKey(url, str_result, fields);
	// cout << "inside _GetMyTrades, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Kucoin::_GetMyTrades", str_result, json_result);

	if (_IsJsonResultValid(json_result)) {}
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Kucoin::ShowMyTrades(string str, int PastDay) {
	// cout << "ShowMyTrades\n";

	if (str == "WatchList") {
		Json::Value::const_iterator iter;
		string line;
  		ifstream myfile ("config/WatchlistKucoin.txt");
  		if (myfile.is_open()) {
    		while ( getline (myfile, line) ) {
				Kucoin::_GetMyTrades(line, PastDay);
				if (_IsJsonResultValid(json_result)) {
					// cout << json_result << endl;
					Json::Value json_items = json_result["data"]["items"];
					for (Json::Value::const_iterator iter = json_items.begin(); iter != json_items.end(); iter++)
						cout << "symbol: " << YELLOW((*iter)["symbol"]) << ", price: " << YELLOW((*iter)["price"]) <<
								", size :" << (*iter)["size"] << ", side: " << (*iter)["side"] << endl;
				}
				else {
					cout << "json_result is not valid\n";
					cout << json_result << endl;
				}
			}
    		cout << endl;
    		myfile.close();
    	}
    	else {
    		cout << "myfile is not open\n";
    		return;
    	}
	}
	else {
		Kucoin::_GetMyTrades(str, PastDay);
		if (_IsJsonResultValid(json_result)) {
			// cout << json_result << endl;
			Json::Value json_items = json_result["data"]["items"];
			for (Json::Value::const_iterator iter = json_items.begin(); iter != json_items.end(); iter++)
				cout << "symbol: " << YELLOW((*iter)["symbol"]) << ", price: " << YELLOW((*iter)["price"]) <<
						", size :" << (*iter)["size"] << ", side: " << (*iter)["side"] << endl;
		}
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
		}
	}
}

void Kucoin::ShowTradesPerformance(string &str, int PastDay) {
	// cout << "ShowTradesPerformance\n";

	double Sells = 0, Buys = 0;


	if (str == "WatchList") {
		Json::Value::const_iterator iter;
		string line;
  		ifstream myfile ("config/WatchlistKucoin.txt");
  		if (myfile.is_open()) {
    		while ( getline (myfile, line) ) {
				Kucoin::_GetMyTrades(line, PastDay);
				// cout << json_result << endl;

				if (_IsJsonResultValid(json_result)) {
					Json::Value json_items = json_result["data"]["items"];
					for (Json::Value::const_iterator iter = json_items.begin(); iter != json_items.end(); iter++) {
						cout << "symbol: " << YELLOW((*iter)["symbol"]) << ", price: " << YELLOW((*iter)["price"]) <<
								", size :" << (*iter)["size"] << ", side: " << (*iter)["side"] << endl;
						double quoteQty = Utility::JsonToDouble((*iter)["size"]) * Utility::JsonToDouble((*iter)["price"]);
						if ((*iter)["side"].asString() == "buy") 
							Sells += quoteQty*0.999;
						else
							Buys += quoteQty*1.001;
					}
				}
				else {
					cout << "json_result is not valid\n";
					cout << json_result << endl;
					return;
				}
			}
    		cout << endl;
    		myfile.close();
    	}
    	else {
    		cout << "myfile is not open\n";
    		return;
    	}
	}
	else {
		Kucoin::_GetMyTrades(str, PastDay);
		// cout << json_result << endl;

		if (_IsJsonResultValid(json_result)) {
			Json::Value json_items = json_result["data"]["items"];
			for (Json::Value::const_iterator iter = json_items.begin(); iter != json_items.end(); iter++) {
				cout << "symbol: " << YELLOW((*iter)["symbol"]) << ", price: " << YELLOW((*iter)["price"]) <<
						", size :" << (*iter)["size"] << ", side: " << (*iter)["side"] << endl;
				double quoteQty = Utility::JsonToDouble((*iter)["size"]) * Utility::JsonToDouble((*iter)["price"]);
				if ((*iter)["side"].asString() == "buy") 
					Sells += quoteQty*0.999;
				else
					Buys += quoteQty*1.001;
			}
		}
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
			return;
		}
	}

	cout << "Considering all trades by " << YELLOW(str) << ". Buys " << Buys << " and sells " << Sells << ". ";
	if (Sells > Buys)
		cout << GREEN(Sells-Buys) << " ( " << GREEN((Sells-Buys)/Buys*100) << GREEN (" %") <<  " )\n";
	else
		cout << RED(Sells-Buys) << " ( " << RED((Sells-Buys)/Buys*100) << RED (" %") <<  " )\n";
}

void Kucoin::ShowDepositAddress(string &str) {
	// cout << "ShowDepositAddress\n";
	string url, querystring;
	string endpoint = "/api/v1/deposit-addresses?";
	
	string temp;
	_pathQueryStringToUrl(url, endpoint, querystring, temp, temp, str, 0);
	// cout << "inside ShowDepositAddress, url: " << url << endl;
	
	StructKucoinHttpFields fields;
	_FillKucoinFields(fields, querystring, "GET", temp);

	string str_result;
	CurlAPI::CurlGetRequestWithKCKey(url, str_result, fields);
	// cout << "inside ShowDepositAddress, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Kucoin::ShowDepositAddress", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Kucoin::ShowDepositHistory(string str) {
	// cout << "ShowDepositHistory\n";
	string url, querystring;
	string endpoint = "/api/v1/deposits";
	if (str != "All")
		endpoint += "?";

	string temp;
	if (str == "All")
		_pathQueryStringToUrl(url, endpoint, querystring, temp, temp, temp, 0);
	else
		_pathQueryStringToUrl(url, endpoint, querystring, temp, temp, str, 0);
	// cout << "inside ShowDepositHistory, url: " << url << endl;
	
	StructKucoinHttpFields fields;
	_FillKucoinFields(fields, querystring, "GET", temp);

	string str_result;
	CurlAPI::CurlGetRequestWithKCKey(url, str_result, fields);
	// cout << "inside ShowDepositHistory, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Kucoin::ShowDepositHistory", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Kucoin::ShowWithdrawHistory(string str) {
	// cout << "ShowWithdrawHistory\n";
	string url, querystring;
	string endpoint = "/api/v1/hist-withdrawals";
	if (str != "All")
		endpoint += "?";

	string temp;
	if (str == "All")
		_pathQueryStringToUrl(url, endpoint, querystring, temp, temp, temp, 0);
	else
		_pathQueryStringToUrl(url, endpoint, querystring, temp, temp, str, 0);
	// cout << "inside ShowWithdrawHistory, url: " << url << endl;
	
	StructKucoinHttpFields fields;
	_FillKucoinFields(fields, querystring, "GET", temp);

	string str_result;
	CurlAPI::CurlGetRequestWithKCKey(url, str_result, fields);
	// cout << "inside ShowWithdrawHistory, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Kucoin::ShowWithdrawHistory", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Kucoin::SendOrder(string symbol, string side, string type, double size, double price) {
	cout << "symbol: " << symbol << ", side: " << side << ", type: " << type << ", size: " << size << ", price: " << price << endl << endl;

	if (Utility::AreYouSure("")) {
		string url(KUCOIN_HOST);  
		string endpoint = "/api/v1/orders";
		url += endpoint;

		string postData;
		string temp;
		Kucoin::_createJsonData(postData, symbol, temp, temp, temp, temp, 0, 0, 0, 0);
		// Kucoin::_createJsonData(postData, symbol, temp, side, temp, temp, size, 0, price, 0);

		StructKucoinHttpFields fields;
		_FillKucoinFields(fields, endpoint, "POST", postData);

		string str_result;
		CurlAPI::CurlPostRequestWithKCKey(url, str_result, fields, postData);
		// cout << "inside SendOrder, str_result: " << str_result << endl;

		Utility::ParseStringToJson("Kucoin::SendOrder", str_result, json_result);

		if (_IsJsonResultValid(json_result))
			cout << json_result << endl;
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
		}
		exit(0);
	}
}

void Kucoin::CancelOrder(string symbol, string orderId) {
	cout << "symbol: " << symbol << ", orderId: " << orderId << endl << endl;

	string url(KUCOIN_HOST); 
	string querystring = "/api/v1/orders/";
	querystring += orderId;
	url += querystring;

	string temp;
	StructKucoinHttpFields fields;
	_FillKucoinFields(fields, querystring, "DELETE", temp);

	string str_result;
	CurlAPI::CurlDeleteRequestWithKCKey(url, str_result, fields);
	// cout << "inside ShowDepositAddress, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Kucoin::CancelOrder", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}	

void Kucoin::CancelAllOrders(string symbol) {
	cout << "symbol: " << symbol << endl << endl;

	string url, querystring;
	string endpoint = "/api/v1/orders";
	if (symbol != "All")
		endpoint += "?";
	
	string temp;
	if (symbol == "All")
		_pathQueryStringToUrl(url, endpoint, querystring, temp, temp, temp, 0);
	else 
		_pathQueryStringToUrl(url, endpoint, querystring, temp, temp, symbol, 0);
	cout << "inside CancelAllOrders, url: " << url << endl;
	
	StructKucoinHttpFields fields;
	_FillKucoinFields(fields, querystring, "DELETE", temp);

	string str_result;
	CurlAPI::CurlDeleteRequestWithKCKey(url, str_result, fields);
	// cout << "inside CancelAllOrders, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Kucoin::CancelAllOrders", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Kucoin::TransferBetweenBankAndExchange(string currency, double amount, string type) {
	cout << "currency: " << currency << ", type: " << type << ", amount: " << amount << endl;

	string url(KUCOIN_HOST);  
	string endpoint = "/api/v2/accounts/inner-transfer";
	url += endpoint;

	string from, to;
	if (type == "bankToExchange") {
		from = "main";
		to = "trade";
	}
	else if (type == "exchangeToBank") {
		from = "trade";
		to = "main";
	}
	else {
		cout << "Invalid type " << type << endl;
		return;
	}

	string postData;
	string temp;
	Kucoin::_createJsonData(postData, temp, currency, temp, from, to, 0, amount, 0, 0);

	StructKucoinHttpFields fields;
	_FillKucoinFields(fields, endpoint, "POST", postData);

	string str_result;
	CurlAPI::CurlPostRequestWithKCKey(url, str_result, fields, postData);
	// cout << "inside TransferBetweenBankAndExchange, str_result: " << str_result << endl;

	Utility::ParseStringToJson("TransferBetweenBankAndExchange", str_result, json_result);
	
	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
	exit(0);
}
