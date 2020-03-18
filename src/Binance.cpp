#include "Binance.h"
#include "CurlAPI.h"
#include "binacpp_utils.h"
#include <sstream>


using namespace std;

Binance* Binance::instance = 0;
Binance* Binance::getInstance() {
    if (instance == 0) {
        instance = new Binance();
    }
    return instance;
}


int myRecvWindow = 15000;


static bool _IsJsonResultValid(Json::Value json_result) {
	// cout << json_result << endl;
	if (json_result.isObject()) {
		if (json_result.isMember("code")) {
			cout << "json_result is invalid: " << json_result << endl;
			return false;
		}
	}

	if (json_result.isNull()) {
		cout << "json_result is invalid: " << json_result << endl;
		return false;
	}

	return true;
} 

// symbol is necessary
int Binance::_pathQueryStringToUrl(	string &url, string baseAddress, string symbol, string asset, 
									int limit, long fromId, long recvWindow, int status, long startTime, long endTime) {
	url = BINANCE_HOST;
	url += baseAddress;

	string querystring(""); 

	if (symbol.size() != 0) {
		querystring.append("&symbol=");
		querystring.append(symbol);
	}

	if (asset.size() != 0) {
		querystring.append("&asset=");
		querystring.append(asset);
	}
	
	if ( limit > 0 ) {
		querystring.append("&limit=");
		querystring.append( to_string( limit ) );
	}

	if ( fromId > 0 ) {
		querystring.append("&fromId=");
		querystring.append( to_string( fromId ) );
	}

	if ( recvWindow > 0 ) {
		querystring.append("&recvWindow=");
		querystring.append( to_string( recvWindow ) );
	}

	if ( status > 0 ) {
		querystring.append("&status=");
		querystring.append( to_string( status ) );
	}

	if ( startTime > 0 ) {
		querystring.append("&startTime=");
		querystring.append( to_string( startTime ) );
	}

	if ( endTime > 0 ) {
		querystring.append("&endTime=");
		querystring.append( to_string( endTime ) );
	}


	querystring.append("&timestamp=");
	querystring.append(to_string( get_current_ms_epoch()));

	string signature = hmac_sha256(secret_key.c_str(), querystring.c_str());
	querystring.append("&signature=");
	querystring.append(signature);

	url.append(querystring);

	// cout << "url: " << url << endl;
	return 0;
}

int Binance::_pathQueryStringToUrl_2(string &url, string baseAddress, string symbol, string side, string type, 
									 string timeInForce, double quantity, double price, double stopPrice, 
									 double stopLimitPrice, long orderId, long recvWindow) {
	url = BINANCE_HOST;
	url += baseAddress;

	string querystring(""); 

	if (symbol.size() != 0) {
		querystring.append("&symbol=");
		querystring.append(symbol);
	}

	if (side.size() != 0) {
		querystring.append("&side=");
		querystring.append(side);
	}

	if (type.size() != 0 && type != "OCO") {
		querystring.append("&type=");
		querystring.append(type);
	}

	if (timeInForce.size() != 0) {
		if (type != "OCO") 
			querystring.append("&timeInForce=");
		else
			querystring.append("&stopLimitTimeInForce=");
		querystring.append("GTC");
	}
	
	if (quantity > 0) {
		querystring.append("&quantity=");
		querystring.append(to_string(quantity));
	}
	
	if (price > 0) {
		if (type == "LIMIT" || type == "OCO")
			querystring.append("&price=");
		else if (type == "STOP_LOSS") 
			querystring.append("&stopPrice=");
		else if (type == "STOP_LOSS_LIMIT") {
			querystring.append("&stopPrice=");
			querystring.append(to_string(price));
			querystring.append("&price=");
			price = price * 0.99;
			Utility::RoundPriceBasedOnPair(symbol, price);
		}
		else {
			cout << RED ("Invalid type: " + type + "\n");
			return -1;
		}

		querystring.append(Utility::tostring(price));
	}

	if (stopPrice > 0) {
		querystring.append("&stopPrice=");
		querystring.append(to_string(stopPrice));
	}

	if (stopLimitPrice > 0) {
		querystring.append("&stopLimitPrice=");
		querystring.append(to_string(stopLimitPrice));
	}
	
	if (orderId > 0) {
		querystring.append("&orderId=");
		querystring.append(to_string(orderId));
	}

	if (recvWindow > 0 ) {
		querystring.append("&recvWindow=");
		querystring.append( to_string( recvWindow ) );
	}
	
	querystring.append("&timestamp=");
	querystring.append(to_string( get_current_ms_epoch()));

	string signature = hmac_sha256(secret_key.c_str(), querystring.c_str());
	querystring.append("&signature=");
	querystring.append(signature);

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

void Binance::Init() {
	Exchange::setKeyFilePath("config/BinanceKeys.txt");
	Exchange::InitApiSecret();
}

void Binance::Cleanup() {
	Exchange::Cleanup();
	// cout << "Successfully perform Binance cleaning up\n";
}

void Binance::ShowServerTime() {
	// cout << "Binance::ShowServerTime\n";
	string url(BINANCE_HOST);  
	url += "/api/v1/time";

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside ShowServerTime, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::ShowServerTime", str_result, json_result);

	if (_IsJsonResultValid(json_result)) 
		// cout << json_result << endl;
		cout << fastWriter.write(json_result) << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Binance::TestConnectivity() {
	// cout << "Binance::TestConnectivity\n";
	string url(BINANCE_HOST);  
	url += "/api/v1/ping";

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside TestConnectivity, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::TestConnectivity", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		// cout << json_result << endl;
		cout << fastWriter.write(json_result) << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Binance::ShowAccountStatus() {
	// cout << "Binance::ShowAccountStatus\n";
	string url;
	string baseAddress = "/wapi/v3/accountStatus.html?";

	string temp; 
	_pathQueryStringToUrl(url, baseAddress, temp, temp, 0, 0, myRecvWindow, 0, 0, 0); 

	string str_result;
	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
	// cout << "inside ShowAccountStatus, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::ShowAccountStatus", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
		// cout << fastWriter.write(json_result) << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Binance::ShowExchangeInfo() {
	// cout << "Binance::ShowExchangeInfo\n";
	string url(BINANCE_HOST);  
	url += "/api/v1/exchangeInfo";

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside ShowExchangeInfo, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::ShowExchangeInfo", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
		// cout << fastWriter.write(json_result) << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Binance::_GetAllPrices() {
	// cout << "GetAllPrices\n";
	string url(BINANCE_HOST);  
	url += "/api/v3/ticker/price";

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside GetAllPrices, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::_GetAllPrices", str_result, json_result);
	if (_IsJsonResultValid(json_result)) {}
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Binance::_GetPriceBySymbol(string &symbol) {
	// cout << symbol << " - _GetPriceBySymbol\n";
	string url(BINANCE_HOST);  
	transform(symbol.begin(), symbol.end(), symbol.begin(), ::toupper);
	url += "/api/v3/ticker/price?symbol="+symbol;

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside _GetPriceBySymbol, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::_GetPriceBySymbol", str_result, json_result);

	if (_IsJsonResultValid(json_result)) {}
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Binance::GetPrices(string &str, SymbolPriceSrtuct* result, int &len) {
	int counter = 1;

	if (str == "All") {
		Binance::_GetAllPrices();
		if (_IsJsonResultValid(json_result)) {
			int cnt = 0;
			for (Json::Value::const_iterator iter = json_result.begin(); iter != json_result.end(); iter++) {
				result[cnt].symbol = (*iter)["symbol"].asString();
				result[cnt].price = Utility::JsonToDouble((*iter)["price"]);
				cnt++;
			}
			len = cnt;
		}
	}
	else if (str == "WatchList") {
		Json::Value::const_iterator iter;
		Binance::_GetAllPrices();
		string line;
  		ifstream myfile ("config/WatchlistBinance.txt");
  		if (myfile.is_open()) {
  			int cnt = 0;
    		while ( getline (myfile, line) ) {
    			if (_IsJsonResultValid(json_result)) {
    				for (iter = json_result.begin(); iter != json_result.end(); iter++)
						if ((*iter)["symbol"] == line) {
							// cout << *iter << endl;
							// cout << fastWriter.write(*iter);
							result[cnt].symbol = (*iter)["symbol"].asString();
							result[cnt].price = Utility::JsonToDouble((*iter)["price"]);
							cnt++;
						}
    			}
				else {
					cout << "json_result is not valid\n";
					cout << json_result << endl;
				}
    		}
    		len = cnt;
    		myfile.close();
    	}
    	else
    		cout << "myfile is not open\n";
	}
	else {
		Binance::_GetPriceBySymbol(str);
		if (_IsJsonResultValid(json_result)) {
			result[0].symbol = json_result["symbol"].asString();
			result[0].price = Utility::JsonToDouble(json_result["price"]);
			len = 1;
		}
	}
}

void Binance::ShowPrices(string str) {
	Exchange::ShowPrices(str);
}

void Binance::_GetAccountInfoBalances() {
	string url;
	string baseAddress;

	baseAddress = "/api/v3/account?";

	string temp;
	_pathQueryStringToUrl(url, baseAddress, temp, temp, 0, 0, myRecvWindow, 0, 0, 0); 
	// cout << "Inside _GetAccountInfoBalances, url: " << url << endl;

	string str_result;
	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
	// cout << "inside _GetAccountInfoBalances, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::_GetAccountInfoBalances", str_result, json_result);

	if (_IsJsonResultValid(json_result)) {}
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

// mode is not used in Binance exchange and is added for compatibility
map <string, map<string, double>> Binance::GetBalances(_GetBalancesModes mode) {
	// cout << "GetBalances\n";

	Binance::_GetAccountInfoBalances();
	
	map < string, map <string,double> >  userBalance;

	if (_IsJsonResultValid(json_result)) {
		if (json_result["balances"].size() == 0)
			cout << json_result << endl;

		for ( int i  = 0 ; i < json_result["balances"].size() ; i++ ) {
			string symbol = json_result["balances"][i]["asset"].asString();
			userBalance[symbol]["f"] = atof( json_result["balances"][i]["free"].asString().c_str() );
			userBalance[symbol]["l"] = atof( json_result["balances"][i]["locked"].asString().c_str() );
		}
	}
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}

	return userBalance;
}

void Binance::ShowBalances() {
	Exchange::ShowBalances();
}

map <string, StructBalanceInUSDT> Binance::ShowBalanceInUSDT() {
	map <string, StructBalanceInUSDT> balanceResult;

	// Get balance
	map <string, map<string,double>> userBalance;
	userBalance = GetBalances(BANK_AND_EXCHANGE_MODE);

	// Get current prices
	string str = "All";
	SymbolPriceSrtuct result[2000];
	int len;
	Binance::GetPrices(str, result, len);

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
					if (result[i].symbol == (symbol+"USDT"))
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
	// cout << "userBalance.size: " << userBalance.size() << ", jsonBinance.size: " << jsonBinance.size() << endl;

	return balanceResult;
}

bool Binance::GetOpenOrders(string &str, Json::Value &jsonOpenOrders) {
	string url;
	string baseAddress;

	baseAddress = "/api/v3/openOrders?";

	string temp;
	if (str == "All")
		_pathQueryStringToUrl(url, baseAddress, temp, temp, 0, 0, myRecvWindow, 0, 0, 0); 
	else
		_pathQueryStringToUrl(url, baseAddress, str, temp, 0, 0, myRecvWindow, 0, 0, 0); 
	// cout << "Inside GetOpenOrders, url: " << url << endl;

	string str_result;
	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
	// cout << "inside GetOpenOrders, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::GetOpenOrders", str_result, jsonOpenOrders);
	// // if (jsonOpenOrders.isArray())
	// // 	return true;
	// // else 
	// // 	return false;
	// return jsonOpenOrders.isArray();
	
	// return _IsJsonResultValid(jsonOpenOrders);
	if (_IsJsonResultValid(jsonOpenOrders)) 
		return true;
	else {
		cout << "jsonOpenOrders is not valid\n";
		cout << jsonOpenOrders << endl;
		return false;
	}
}

void Binance::_GetAllOrders(string &str, int PastDay) {
	string url;
	string baseAddress;

	baseAddress = "/api/v3/allOrders?";

	long StartTime;
	_GetStartTimeFromDay(PastDay, StartTime);

	string temp;
	if (str == "All")
		_pathQueryStringToUrl(url, baseAddress, temp, temp, 0, 0, myRecvWindow, 0, StartTime, 0); 
	else
		_pathQueryStringToUrl(url, baseAddress, str, temp, 0, 0, myRecvWindow, 0, StartTime, 0); 
	
	string str_result;
	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
	// cout << "inside _GetAllOrders, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::_GetAllOrders", str_result, json_result);

	if (_IsJsonResultValid(json_result)) {}
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Binance::ShowOpenOrders(string str) {
	// cout << "ShowOpenOrders\n";
	
	if (Binance::GetOpenOrders(str, json_result)) {
		for (int i=0; i<json_result.size(); i++) {
			if (json_result[i]["type"].asString() != "STOP_LOSS_LIMIT")
				cout << "symbol: " << YELLOW (json_result[i]["symbol"]) << ", price: " << YELLOW (json_result[i]["price"]) << 
						", side: " << json_result[i]["side"] <<	", origQty: " << json_result[i]["origQty"] << ", exec: " << json_result[i]["executedQty"] <<
						", type: " << json_result[i]["type"] << ", orderId: " << json_result[i]["orderId"] << endl;
			else
				cout << "symbol: " << YELLOW (json_result[i]["symbol"]) << ", stop: " << YELLOW (json_result[i]["stopPrice"]) << ", price: " << YELLOW (json_result[i]["price"]) << 
						", side: " << json_result[i]["side"] <<	", origQty: " << json_result[i]["origQty"] <<
						", orderId: " << json_result[i]["orderId"] << endl;
		}
	}
	else
		cout << "Cannot GetOpenOrders\n";
}

void Binance::ShowAllOrders(string &str, int PastDay) {
	// cout << "ShowAllOrders\n";
	
	if (str == "WatchList") {
		Json::Value::const_iterator iter;
		string line;
  		ifstream myfile ("config/WatchlistBinance.txt");
  		if (myfile.is_open()) {
    		while ( getline (myfile, line) ) {
				Binance::_GetAllOrders(line, PastDay);
				// if (json_result.isArray())
				if (_IsJsonResultValid(json_result))
					for (int i=0 ; i<json_result.size(); i++)
					// if (json_result[i].isMember("symbol"))
						cout << "symbol: " << KYEL << json_result[i]["symbol"] << RESET << ", side: " << json_result[i]["side"] << 
								", price: " << KYEL << json_result[i]["price"] << RESET << 	", origQty :" << json_result[i]["origQty"] << 
								", status: " << json_result[i]["status"] << endl;
				else {
					cout << "json_result is not valid\n";
					cout << json_result << endl;
				}
			}
    		cout << endl;
    		myfile.close();
    	}
	}
	else {
		Binance::_GetAllOrders(str, PastDay);
		// cout << json_result << endl;
		// if (json_result.isArray())
		if (_IsJsonResultValid(json_result))
			for (int i=0 ; i<json_result.size(); i++)
			// if (json_result[i].isMember("symbol"))
				cout << "symbol: " << KYEL << json_result[i]["symbol"] << RESET << ", side: " << json_result[i]["side"] << 
						", price: " << KYEL << json_result[i]["price"] << RESET << ", origQty :" << json_result[i]["origQty"] << 
						", status: " << json_result[i]["status"] << endl;
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
		}
	}
}

void Binance::_GetMyTrades(string &str, int PastDay) {
	string url;
	string baseAddress = "/api/v3/myTrades?";

	long StartTime;
	_GetStartTimeFromDay(PastDay, StartTime);

	string temp;
	_pathQueryStringToUrl(url, baseAddress, str, temp, 0, 0, myRecvWindow, 0, StartTime, 0); 
	// cout << "<BinaCPP::get_account> url = " << url << endl;

	string str_result;
	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
	// cout << "inside _GetMyTrades, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::_GetMyTrades", str_result, json_result);

	if (_IsJsonResultValid(json_result)) {}
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Binance::ShowMyTrades(string str, int PastDay) {
	// cout << "ShowMyTrades\n";

	if (str == "WatchList") {
		Json::Value::const_iterator iter;
		string line;
  		ifstream myfile ("config/WatchlistBinance.txt");
  		if (myfile.is_open()) {
    		while ( getline (myfile, line) ) {
				Binance::_GetMyTrades(line, PastDay);
				if (_IsJsonResultValid(json_result))
					// cout << json_result << endl;
					for (int i=0 ; i<json_result.size(); i++)
						cout << "symbol: " << KYEL << json_result[i]["symbol"] << RESET << 
								", price: " << KYEL << json_result[i]["price"] << RESET << ", qty :" << json_result[i]["qty"] << 
								", quoteQty: " << json_result[i]["quoteQty"] << ", isBuyer: " << json_result[i]["isBuyer"] << endl;
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
		Binance::_GetMyTrades(str, PastDay);
		// cout << json_result << endl;
		if (_IsJsonResultValid(json_result))
			for (int i=0 ; i<json_result.size(); i++)
				cout << "symbol: " << KYEL << json_result[i]["symbol"] << RESET << ", price: " << KYEL << json_result[i]["price"] << RESET << 
						", qty :" << json_result[i]["qty"] << ", quoteQty: " << json_result[i]["quoteQty"] << 
						", isBuyer: " << json_result[i]["isBuyer"] << endl;
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
		}
	}
}

void Binance::ShowTradesPerformance(string &str, int PastDay) {
	// cout << "ShowTradesPerformance\n";

	double Sells = 0, Buys = 0;

	if (str == "WatchList") {
		Json::Value::const_iterator iter;
		string line;
  		ifstream myfile ("config/WatchlistBinance.txt");
  		if (myfile.is_open()) {
    		while ( getline (myfile, line) ) {
				Binance::_GetMyTrades(line, PastDay);
				// cout << json_result << endl;

				if (_IsJsonResultValid(json_result)) {
					for (int i=0 ; i<json_result.size(); i++) {
						cout << "symbol: " << YELLOW(json_result[i]["symbol"]) << ", price: " << YELLOW(json_result[i]["price"]) << 
								", qty :" << json_result[i]["qty"] << ", quoteQty: " << json_result[i]["quoteQty"] << 
								", isBuyer: " << json_result[i]["isBuyer"] << endl;
						if (json_result[i]["isBuyer"].asString() == "false") 
							Sells += atof(json_result[i]["quoteQty"].asString().c_str())*0.999;
						else
							Buys += atof(json_result[i]["quoteQty"].asString().c_str());
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
		Binance::_GetMyTrades(str, PastDay);
		// cout << json_result << endl;

		if (_IsJsonResultValid(json_result)) {
			for (int i=0 ; i<json_result.size(); i++) {
				cout << "symbol: " << YELLOW(json_result[i]["symbol"]) << ", price: " << YELLOW(json_result[i]["price"]) << 
						", qty :" << json_result[i]["qty"] << ", quoteQty: " << json_result[i]["quoteQty"] << 
						", isBuyer: " << json_result[i]["isBuyer"] << endl;
				if (json_result[i]["isBuyer"].asString() == "false") 
					Sells += atof(json_result[i]["quoteQty"].asString().c_str())*0.999;
				else
					Buys += atof(json_result[i]["quoteQty"].asString().c_str());
			}
		}
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
			return;
		}		
	}




	// Binance::_GetMyTrades(str, PastDay);
	// // cout << json_result << endl;

	// if (_IsJsonResultValid(json_result)) {
	// 	for (int i=0 ; i<json_result.size(); i++) {
	// 		cout << "symbol: " << YELLOW(json_result[i]["symbol"]) << ", price: " << YELLOW(json_result[i]["price"]) << 
	// 				", qty :" << json_result[i]["qty"] << ", quoteQty: " << json_result[i]["quoteQty"] << 
	// 				", isBuyer: " << json_result[i]["isBuyer"] << endl;
	// 		if (json_result[i]["isBuyer"].asString() == "false") 
	// 			Sells += atof(json_result[i]["quoteQty"].asString().c_str())*0.999;
	// 		else
	// 			Buys += atof(json_result[i]["quoteQty"].asString().c_str());
	// 	}
	// }
	// else {
	// 	cout << "json_result is not valid\n";
	// 	cout << json_result << endl;
	// 	return;
	// }

	cout << "Considering all trades by " << YELLOW(str) << ". Buys " << Buys << " and sells " << Sells << ". ";
	if (Sells > Buys)
		cout << GREEN(Sells-Buys) << " ( " << GREEN((Sells-Buys)/Buys*100) << GREEN (" %") <<  " )\n";
	else
		cout << RED(Sells-Buys) << " ( " << RED((Sells-Buys)/Buys*100) << RED (" %") <<  " )\n";
}

void Binance::ShowDepositAddress(string &str) {
	// cout << "ShowDepositAddress\n";

	string url;
	string baseAddress;

	baseAddress = "/wapi/v3/depositAddress.html?";
	
	string temp;
	_pathQueryStringToUrl(url, baseAddress, temp, str, 0, 0, myRecvWindow, 0, 0, 0); 
	// cout << "Inside _GetAccountInfoBalances, url: " << url << endl;

	string str_result;
	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
	// cout << "inside ShowDepositAddress, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::ShowDepositAddress", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Binance::ShowDepositHistory(string str) {
	// cout << "ShowDepositHistory\n";

	string url;
	string baseAddress = "/wapi/v3/depositHistory.html?";

	string asset;
	if (str == "All") {}
	else 
		asset = str;

	string temp;
	_pathQueryStringToUrl(url, baseAddress, temp, asset, 0, 0, myRecvWindow, 0, 0, 0); 
	// cout << "url: " <<url << endl;	

	string str_result;
	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
	// cout << "inside ShowDepositHistory, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::ShowDepositHistory", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Binance::ShowWithdrawHistory(string str) {
	// cout << "ShowWithdrawHistory\n";

	string url;
	string baseAddress = "/wapi/v3/withdrawHistory.html?";

	string asset;
	if (str == "All") {}
	else 
		asset = str;

	string temp;
	_pathQueryStringToUrl(url, baseAddress, temp, asset, 0, 0, myRecvWindow, 0, 0, 0); 
	// cout << "url: " <<url << endl;	

	string str_result;
	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
	// cout << "inside ShowDepositHistory, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::ShowWithdrawHistory", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void Binance::SendOrder(string symbol, string side, string type, double quantity, 
						double price, double stopPrice, double stopLimitPrice) {
	cout << "symbol: " << symbol << ", side: " << side << ", type: " << type << ", quantity: " << quantity << ", price: " << price << ", stopPrice: " << stopPrice << ", stopLimitPrice: " << stopLimitPrice << endl << endl;

	if (Utility::AreYouSure("")) {
		string url;
		string baseAddress;
		if (type != "OCO")
			baseAddress = "/api/v3/order?";
		else 
			baseAddress = "/api/v3/order/oco?";


		string temp;
		if (type == "LIMIT" || type == "STOP_LOSS_LIMIT")
			_pathQueryStringToUrl_2(url, baseAddress, symbol, side, type, "GTC", quantity, price, 0, 0, 0, myRecvWindow);
		else if (type == "MARKET")
			_pathQueryStringToUrl_2(url, baseAddress, symbol, side, type, temp, quantity, 0, 0, 0, 0, myRecvWindow);
		else if (type == "STOP_LOSS")
			_pathQueryStringToUrl_2(url, baseAddress, symbol, side, type, temp, quantity, price, 0, 0, 0, myRecvWindow);
		else if (type == "OCO")
			_pathQueryStringToUrl_2(url, baseAddress, symbol, side, type, "GTC", quantity, price, stopPrice, stopLimitPrice, 0, myRecvWindow);
		else {
			cout << "Invalid type " << type << endl;
		}	
		cout << "url: " << url << endl;	

		string str_result;
		CurlAPI::CurlPostRequestWithMBXKey(url, str_result, api_key);
		// cout << "inside SendOrder, str_result: " << str_result << endl;

		Utility::ParseStringToJson("Binance::SendOrder", str_result, json_result);

		if (_IsJsonResultValid(json_result))
			cout << json_result << endl;
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
		}
		exit(0);
	}
}

void Binance::CancelOrder(string symbol, long orderId) {
	cout << "symbol: " << symbol << ", orderId: " << orderId << endl << endl;

	string url;
	string baseAddress = "/api/v3/order?";

	string temp;
	_pathQueryStringToUrl_2(url, baseAddress, symbol, temp, temp, temp, 0, 0, 0, 0, orderId, myRecvWindow);
	cout << "url: " << url << endl;

	string str_result;
	CurlAPI::CurlDeleteRequestWithMBXKey(url, str_result, api_key);
	// cout << "inside CancelOrder, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Binance::CancelOrder", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}	


