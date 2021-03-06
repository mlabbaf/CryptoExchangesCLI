#include "Nobitex.h"
#include "CurlAPI.h"
#include "binacpp_utils.h"


using namespace std;

Nobitex* Nobitex::instance = 0;
Nobitex* Nobitex::getInstance() {
    if (instance == 0) {
        instance = new Nobitex();
    }
    return instance;
}


bool Nobitex::isJsonResultValid(Json::Value json_result) {

	if (json_result["status"] != "ok")
		return false;

	return true;
} 

// // symbol is necessary
// int Nobitex::_pathQueryStringToUrl(	string &url, string baseAddress, string symbol, string asset, string coin, string network,
// 									int limit, long fromId, long recvWindow, int status, long startTime, long endTime) {
// 	url = Nobitex_HOST;
// 	url += baseAddress;

// 	string querystring(""); 

// 	if (symbol.size() != 0) {
// 		querystring.append("&symbol=");
// 		querystring.append(symbol);
// 	}

// 	if (asset.size() != 0) {
// 		querystring.append("&asset=");
// 		querystring.append(asset);
// 	}

// 	if (coin.size() != 0) {
// 		querystring.append("&coin=");
// 		querystring.append(coin);
// 	}

// 	if (network.size() != 0) {
// 		querystring.append("&network=");
// 		querystring.append(network);
// 	}
	
// 	if ( limit > 0 ) {
// 		querystring.append("&limit=");
// 		querystring.append( to_string( limit ) );
// 	}

// 	if ( fromId > 0 ) {
// 		querystring.append("&fromId=");
// 		querystring.append( to_string( fromId ) );
// 	}

// 	if ( recvWindow > 0 ) {
// 		querystring.append("&recvWindow=");
// 		querystring.append( to_string( recvWindow ) );
// 	}

// 	if ( status > 0 ) {
// 		querystring.append("&status=");
// 		querystring.append( to_string( status ) );
// 	}

// 	if ( startTime > 0 ) {
// 		querystring.append("&startTime=");
// 		querystring.append( to_string( startTime ) );
// 	}

// 	if ( endTime > 0 ) {
// 		querystring.append("&endTime=");
// 		querystring.append( to_string( endTime ) );
// 	}


// 	querystring.append("&timestamp=");
// 	querystring.append(to_string( get_current_ms_epoch()));

// 	string signature = hmac_sha256(secret_key.c_str(), querystring.c_str());
// 	querystring.append("&signature=");
// 	querystring.append(signature);

// 	url.append(querystring);

// 	// cout << "url: " << url << endl;
// 	return 0;
// }

// int Nobitex::_pathQueryStringToUrl_2(string &url, string baseAddress, string symbol, string side, string type, 
// 									 string timeInForce, double quantity, double price, double stopPrice, 
// 									 double stopLimitPrice, string orderId, long recvWindow) {
// 	url = Nobitex_HOST;
// 	url += baseAddress;

// 	string querystring(""); 

// 	if (symbol.size() != 0) {
// 		querystring.append("&symbol=");
// 		querystring.append(symbol);
// 	}

// 	if (side.size() != 0) {
// 		querystring.append("&side=");
// 		querystring.append(side);
// 	}

// 	if (type.size() != 0 && type != "OCO") {
// 		querystring.append("&type=");
// 		querystring.append(type);
// 	}

// 	if (timeInForce.size() != 0) {
// 		if (type != "OCO") 
// 			querystring.append("&timeInForce=");
// 		else
// 			querystring.append("&stopLimitTimeInForce=");
// 		querystring.append("GTC");
// 	}
	
// 	if (quantity > 0) {
// 		querystring.append("&quantity=");
// 		querystring.append(Utility::tostring(quantity));
// 	}
	
// 	if (price > 0) {
// 		if (type == "LIMIT" || type == "OCO")
// 			querystring.append("&price=");
// 		else if (type == "STOP_LOSS") 
// 			querystring.append("&stopPrice=");
// 		else if (type == "STOP_LOSS_LIMIT") {
// 			querystring.append("&stopPrice=");
// 			querystring.append(to_string(price));
// 			querystring.append("&price=");
// 			price = price * 0.99;
// 			Utility::RoundPriceBasedOnPair(symbol, price);
// 		}
// 		else {
// 			cout << RED ("Invalid type: " + type + "\n");
// 			return -1;
// 		}

// 		querystring.append(Utility::tostring(price));
// 	}

// 	if (stopPrice > 0) {
// 		querystring.append("&stopPrice=");
// 		querystring.append(to_string(stopPrice));
// 	}

// 	if (stopLimitPrice > 0) {
// 		querystring.append("&stopLimitPrice=");
// 		querystring.append(to_string(stopLimitPrice));
// 	}
	
// 	if (orderId.size() != 0) {
// 		querystring.append("&orderId=");
// 		querystring.append(orderId);
// 	}

// 	if (recvWindow > 0 ) {
// 		querystring.append("&recvWindow=");
// 		querystring.append( to_string( recvWindow ) );
// 	}
	
// 	querystring.append("&timestamp=");
// 	querystring.append(to_string( get_current_ms_epoch()));

// 	string signature = hmac_sha256(secret_key.c_str(), querystring.c_str());
// 	querystring.append("&signature=");
// 	querystring.append(signature);

// 	url.append(querystring);

// 	// cout << "url: " << url << endl;
// 	return 0;
// }

// static void _GetStartTimeFromDay(int PastDay, long &StartTime) {
// 	if (PastDay == 0) 	// All times
// 		StartTime = 0;
// 	else {
// 		struct timeval tv;

// 		gettimeofday(&tv, NULL); 

// 		StartTime = (tv.tv_sec*1000+tv.tv_usec/1000) - PastDay*24*60*60*1000;
// 		// cout << "StartTime: " << StartTime << ", PastDay: " << PastDay << endl;
// 	}
// }

void Nobitex::ShowServerTime() {
// 	// cout << "Nobitex::ShowServerTime\n";
// 	string url(Nobitex_HOST);  
// 	url += "/api/v1/time";

// 	string str_result;
// 	CurlAPI::CurlGetRequest(url, str_result);
// 	// cout << "inside ShowServerTime, str_result: " << str_result << endl;

// 	Utility::ParseStringToJson("Nobitex::ShowServerTime", str_result, json_result);

// 	if (isJsonResultValid(json_result)) 
// 		// cout << json_result << endl;
// 		cout << fastWriter.write(json_result) << endl;
// 	else {
// 		cout << "json_result is not valid\n";
// 		cout << json_result << endl;
// 	}
}

// void Nobitex::TestConnectivity() {
// 	// cout << "Nobitex::TestConnectivity\n";
// 	string url(Nobitex_HOST);  
// 	url += "/api/v1/ping";

// 	string str_result;
// 	CurlAPI::CurlGetRequest(url, str_result);
// 	// cout << "inside TestConnectivity, str_result: " << str_result << endl;

// 	Utility::ParseStringToJson("Nobitex::TestConnectivity", str_result, json_result);

// 	if (isJsonResultValid(json_result))
// 		// cout << json_result << endl;
// 		cout << fastWriter.write(json_result) << endl;
// 	else {
// 		cout << "json_result is not valid\n";
// 		cout << json_result << endl;
// 	}
// }

// void Nobitex::ShowAccountStatus() {
// 	// cout << "Nobitex::ShowAccountStatus\n";
// 	string url;
// 	string baseAddress = "/wapi/v3/accountStatus.html?";

// 	string temp; 
// 	_pathQueryStringToUrl(url, baseAddress, temp, temp, temp, temp, 0, 0, myRecvWindow, 0, 0, 0); 

// 	string str_result;
// 	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
// 	// cout << "inside ShowAccountStatus, str_result: " << str_result << endl;

// 	Utility::ParseStringToJson("Nobitex::ShowAccountStatus", str_result, json_result);

// 	if (isJsonResultValid(json_result))
// 		cout << json_result << endl;
// 		// cout << fastWriter.write(json_result) << endl;
// 	else {
// 		cout << "json_result is not valid\n";
// 		cout << json_result << endl;
// 	}
// }

// void Nobitex::ShowExchangeInfo() {
// 	// cout << "Nobitex::ShowExchangeInfo\n";
// 	string url(Nobitex_HOST);  
// 	url += "/api/v1/exchangeInfo";

// 	string str_result;
// 	CurlAPI::CurlGetRequest(url, str_result);
// 	// cout << "inside ShowExchangeInfo, str_result: " << str_result << endl;

// 	Utility::ParseStringToJson("Nobitex::ShowExchangeInfo", str_result, json_result);

// 	if (isJsonResultValid(json_result))
// 		cout << json_result << endl;
// 		// cout << fastWriter.write(json_result) << endl;
// 	else {
// 		cout << "json_result is not valid\n";
// 		cout << json_result << endl;
// 	}
// }

void Nobitex::getAllPrices(string str, SymbolPriceSrtuct* result, int &len) {
	cout << RED ("Nobitex is not supporting All option\n");
}

void Nobitex::getWatchlistPrices(string str, SymbolPriceSrtuct* result, int &len) {
	string line;
	ifstream myfile (watchlistPath);
	if (myfile.is_open()) {
		int cnt = 0;
		SymbolPriceSrtuct result_temp;
		int len_temp;
		while ( getline (myfile,line) ) {
			Nobitex::getSymbolPrice(line, &result_temp, len_temp);
			if (isJsonResultValid(json_result)) {
				result[cnt].symbol = result_temp.symbol;
				result[cnt++].price = result_temp.price;
			}
		}
		len = cnt;
		myfile.close();
	}
}

void Nobitex::getSymbolPrice(string str, SymbolPriceSrtuct* result, int &len) {
	// cout << str << " - getSymbolPrice\n";
	string url(NOBITEX_HOST);  
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	url += "/v2/trades/"+str;

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside getSymbolPrice, str_result: " << str_result << endl;

	Utility::ParseStringToJson("Nobitex::getSymbolPrice", str_result, json_result);
	// cout << "inside getSymbolPrice, json_result: " << json_result << endl;

	if (isJsonResultValid(json_result)) {
		result[0].symbol = str;
		result[0].price = Utility::JsonToDouble(json_result["trades"][0]["price"]);
		len = 1;
	}
	else
		cout << json_result << endl;
}

// void Nobitex::_GetAccountInfoBalances() {
// 	string url;
// 	string baseAddress;

// 	baseAddress = "/api/v3/account?";

// 	string temp;
// 	_pathQueryStringToUrl(url, baseAddress, temp, temp, temp, temp, 0, 0, myRecvWindow, 0, 0, 0); 
// 	// cout << "Inside _GetAccountInfoBalances, url: " << url << endl;

// 	string str_result;
// 	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
// 	// cout << "inside _GetAccountInfoBalances, str_result: " << str_result << endl;

// 	Utility::ParseStringToJson("Nobitex::_GetAccountInfoBalances", str_result, json_result);

// 	if (isJsonResultValid(json_result)) {}
// 	else {
// 		cout << "json_result is not valid\n";
// 		cout << json_result << endl;
// 	}
// }

// // mode is not used in Nobitex exchange and is added for compatibility
map <string, map<string, double>> Nobitex::GetBalances(_GetBalancesModes mode) {
// 	// cout << "GetBalances\n";

// 	Nobitex::_GetAccountInfoBalances();
	
// 	map < string, map <string,double> >  userBalance;

// 	if (isJsonResultValid(json_result)) {
// 		if (json_result["balances"].size() == 0)
// 			cout << json_result << endl;

// 		for ( int i  = 0 ; i < json_result["balances"].size() ; i++ ) {
// 			string symbol = json_result["balances"][i]["asset"].asString();
// 			userBalance[symbol]["f"] = atof( json_result["balances"][i]["free"].asString().c_str() );
// 			userBalance[symbol]["l"] = atof( json_result["balances"][i]["locked"].asString().c_str() );
// 		}
// 	}
// 	else {
// 		cout << "json_result is not valid\n";
// 		cout << json_result << endl;
// 	}

// 	return userBalance;
}

map <string, StructBalanceInUSDT> Nobitex::ShowBalanceInUSDT() {
// 	map <string, StructBalanceInUSDT> balanceResult;

// 	// Get balance
// 	map <string, map<string,double>> userBalance;
// 	userBalance = GetBalances(BANK_AND_EXCHANGE_MODE);

// 	// Get current prices
// 	string str = "All";
// 	SymbolPriceSrtuct result[2000];
// 	int len;
// 	Nobitex::GetPrices(str, result, len);

// 	// Calculate whole balances
// 	double wholeBalance = 0;
// 	for (map <string, map<string, double>>::iterator it_i=userBalance.begin(); it_i!= userBalance.end(); it_i++) {
// 		string symbol 			= (*it_i).first;
// 		map <string,double> balance 	= (*it_i).second;
// 		double coinBalance = balance["f"] + balance["l"];

// 		if (coinBalance > 0) {
// 			// Get price
// 			double price = -1;
// 			if (symbol == "USDT") 
// 				price = 1;
// 			else
// 				for (int i=0; i<len; i++) {
// 					if (result[i].symbol == (symbol+"USDT"))
// 						price = result[i].price;
// 				}
// 			if (price == -1) {
// 				cout << YELLOW("Warning:") << " cannot find price for " << symbol << endl;
// 				price = 0;
// 				// return;
// 			}

// 			wholeBalance += coinBalance * price;
// 			cout << symbol << ": " << YELLOW(coinBalance) << " (" << balance["f"] << " , " << balance["l"] <<
// 				 ") at price " << YELLOW(price) << " (" << RED(coinBalance*price) << ")\n";

// 			balanceResult[symbol].balance = coinBalance;
// 			balanceResult[symbol].balanceFree = balance["f"];
// 			balanceResult[symbol].balanceLocked = balance["l"];
// 			balanceResult[symbol].price = price;
// 			balanceResult[symbol].asset = coinBalance*price;
// 		}
// 	}

// 	cout << "\nwholeBalance is " << YELLOW(wholeBalance) << " USDT\n\n";
// 	// cout << "userBalance.size: " << userBalance.size() << ", jsonNobitex.size: " << jsonNobitex.size() << endl;

// 	return balanceResult;
}

bool Nobitex::GetOpenOrders(string &str, Json::Value &jsonOpenOrders) {
// 	string url;
// 	string baseAddress;

// 	baseAddress = "/api/v3/openOrders?";

// 	string temp;
// 	if (str == "All")
// 		_pathQueryStringToUrl(url, baseAddress, temp, temp, temp, temp, 0, 0, myRecvWindow, 0, 0, 0); 
// 	else
// 		_pathQueryStringToUrl(url, baseAddress, str, temp, temp, temp, 0, 0, myRecvWindow, 0, 0, 0); 
// 	// cout << "Inside GetOpenOrders, url: " << url << endl;

// 	string str_result;
// 	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
// 	// cout << "inside GetOpenOrders, str_result: " << str_result << endl;

// 	Utility::ParseStringToJson("Nobitex::GetOpenOrders", str_result, jsonOpenOrders);
// 	// // if (jsonOpenOrders.isArray())
// 	// // 	return true;
// 	// // else 
// 	// // 	return false;
// 	// return jsonOpenOrders.isArray();
	
// 	// return isJsonResultValid(jsonOpenOrders);
// 	if (isJsonResultValid(jsonOpenOrders)) 
// 		return true;
// 	else {
// 		cout << "jsonOpenOrders is not valid\n";
// 		cout << jsonOpenOrders << endl;
// 		return false;
// 	}
}


bool Nobitex::GetOpenOrders(string &str, vector <SymbolOrderStruct> &vecOpenOrders) {
	cout << RED("This prototype is only used in Coinex\n");
	return false;
}

// void Nobitex::_GetAllOrders(string &str, int PastDay) {
// 	string url;
// 	string baseAddress;

// 	baseAddress = "/api/v3/allOrders?";

// 	long StartTime;
// 	_GetStartTimeFromDay(PastDay, StartTime);

// 	string temp;
// 	if (str == "All")
// 		_pathQueryStringToUrl(url, baseAddress, temp, temp, temp, temp, 0, 0, myRecvWindow, 0, StartTime, 0); 
// 	else
// 		_pathQueryStringToUrl(url, baseAddress, str, temp, temp, temp, 0, 0, myRecvWindow, 0, StartTime, 0); 
	
// 	string str_result;
// 	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
// 	// cout << "inside _GetAllOrders, str_result: " << str_result << endl;

// 	Utility::ParseStringToJson("Nobitex::_GetAllOrders", str_result, json_result);

// 	if (isJsonResultValid(json_result)) {}
// 	else {
// 		cout << "json_result is not valid\n";
// 		cout << json_result << endl;
// 	}
// }

// void Nobitex::ShowOpenOrders(string str) {
// 	// cout << "ShowOpenOrders\n";
	
// 	if (Nobitex::GetOpenOrders(str, json_result)) {
// 		for (int i=0; i<json_result.size(); i++) {
// 			if (json_result[i]["type"].asString() != "STOP_LOSS_LIMIT")
// 				cout << "symbol: " << YELLOW (json_result[i]["symbol"]) << ", price: " << YELLOW (json_result[i]["price"]) << 
// 						", side: " << json_result[i]["side"] <<	", origQty: " << json_result[i]["origQty"] << ", exec: " << json_result[i]["executedQty"] <<
// 						", type: " << json_result[i]["type"] << ", orderId: " << json_result[i]["orderId"] << endl;
// 			else
// 				cout << "symbol: " << YELLOW (json_result[i]["symbol"]) << ", stop: " << YELLOW (json_result[i]["stopPrice"]) << ", price: " << YELLOW (json_result[i]["price"]) << 
// 						", side: " << json_result[i]["side"] <<	", origQty: " << json_result[i]["origQty"] <<
// 						", orderId: " << json_result[i]["orderId"] << endl;
// 		}
// 	}
// 	else
// 		cout << "Cannot GetOpenOrders\n";
// }

// void Nobitex::ShowAllOrders(string &str, int PastDay) {
// 	// cout << "ShowAllOrders\n";
	
// 	if (str == "WatchList") {
// 		Json::Value::const_iterator iter;
// 		string line;
//   		ifstream myfile ("config/WatchlistNobitex.txt");
//   		if (myfile.is_open()) {
//     		while ( getline (myfile, line) ) {
// 				Nobitex::_GetAllOrders(line, PastDay);
// 				// if (json_result.isArray())
// 				if (isJsonResultValid(json_result))
// 					for (int i=0 ; i<json_result.size(); i++)
// 					// if (json_result[i].isMember("symbol"))
// 						cout << "symbol: " << KYEL << json_result[i]["symbol"] << RESET << ", side: " << json_result[i]["side"] << 
// 								", price: " << KYEL << json_result[i]["price"] << RESET << 	", origQty :" << json_result[i]["origQty"] << 
// 								", status: " << json_result[i]["status"] << endl;
// 				else {
// 					cout << "json_result is not valid\n";
// 					cout << json_result << endl;
// 				}
// 			}
//     		cout << endl;
//     		myfile.close();
//     	}
// 	}
// 	else {
// 		Nobitex::_GetAllOrders(str, PastDay);
// 		// cout << json_result << endl;
// 		// if (json_result.isArray())
// 		if (isJsonResultValid(json_result))
// 			for (int i=0 ; i<json_result.size(); i++)
// 			// if (json_result[i].isMember("symbol"))
// 				cout << "symbol: " << KYEL << json_result[i]["symbol"] << RESET << ", side: " << json_result[i]["side"] << 
// 						", price: " << KYEL << json_result[i]["price"] << RESET << ", origQty :" << json_result[i]["origQty"] << 
// 						", status: " << json_result[i]["status"] << endl;
// 		else {
// 			cout << "json_result is not valid\n";
// 			cout << json_result << endl;
// 		}
// 	}
// }

// void Nobitex::_GetMyTrades(string &str, int PastDay) {
// 	string url;
// 	string baseAddress = "/api/v3/myTrades?";

// 	long StartTime;
// 	_GetStartTimeFromDay(PastDay, StartTime);

// 	string temp;
// 	_pathQueryStringToUrl(url, baseAddress, str, temp, temp, temp, 0, 0, myRecvWindow, 0, StartTime, 0); 
// 	// cout << "<BinaCPP::get_account> url = " << url << endl;

// 	string str_result;
// 	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
// 	// cout << "inside _GetMyTrades, str_result: " << str_result << endl;

// 	Utility::ParseStringToJson("Nobitex::_GetMyTrades", str_result, json_result);

// 	if (isJsonResultValid(json_result)) {}
// 	else {
// 		cout << "json_result is not valid\n";
// 		cout << json_result << endl;
// 	}
// }

// void Nobitex::ShowMyTrades(string str, int PastDay) {
// 	// cout << "ShowMyTrades\n";

// 	if (str == "WatchList") {
// 		Json::Value::const_iterator iter;
// 		string line;
//   		ifstream myfile ("config/WatchlistNobitex.txt");
//   		if (myfile.is_open()) {
//     		while ( getline (myfile, line) ) {
// 				Nobitex::_GetMyTrades(line, PastDay);
// 				if (isJsonResultValid(json_result))
// 					// cout << json_result << endl;
// 					for (int i=0 ; i<json_result.size(); i++)
// 						cout << "symbol: " << KYEL << json_result[i]["symbol"] << RESET << 
// 								", price: " << KYEL << json_result[i]["price"] << RESET << ", qty :" << json_result[i]["qty"] << 
// 								", quoteQty: " << json_result[i]["quoteQty"] << ", isBuyer: " << json_result[i]["isBuyer"] << endl;
// 				else {
// 					cout << "json_result is not valid\n";
// 					cout << json_result << endl;
// 				}
// 			}
//     		cout << endl;
//     		myfile.close();
//     	}
//     	else {
//     		cout << "myfile is not open\n";
//     		return;
//     	}
// 	}
// 	else {
// 		Nobitex::_GetMyTrades(str, PastDay);
// 		// cout << json_result << endl;
// 		if (isJsonResultValid(json_result))
// 			for (int i=0 ; i<json_result.size(); i++)
// 				cout << "symbol: " << KYEL << json_result[i]["symbol"] << RESET << ", price: " << KYEL << json_result[i]["price"] << RESET << 
// 						", qty :" << json_result[i]["qty"] << ", quoteQty: " << json_result[i]["quoteQty"] << 
// 						", isBuyer: " << json_result[i]["isBuyer"] << endl;
// 		else {
// 			cout << "json_result is not valid\n";
// 			cout << json_result << endl;
// 		}
// 	}
// }

// void Nobitex::ShowTradesPerformance(string &str, int PastDay) {
// 	// cout << "ShowTradesPerformance\n";

// 	double Sells = 0, Buys = 0;

// 	if (str == "WatchList") {
// 		Json::Value::const_iterator iter;
// 		string line;
//   		ifstream myfile ("config/WatchlistNobitex.txt");
//   		if (myfile.is_open()) {
//     		while ( getline (myfile, line) ) {
// 				Nobitex::_GetMyTrades(line, PastDay);
// 				// cout << json_result << endl;

// 				if (isJsonResultValid(json_result)) {
// 					for (int i=0 ; i<json_result.size(); i++) {
// 						cout << "symbol: " << YELLOW(json_result[i]["symbol"]) << ", price: " << YELLOW(json_result[i]["price"]) << 
// 								", qty :" << json_result[i]["qty"] << ", quoteQty: " << json_result[i]["quoteQty"] << 
// 								", isBuyer: " << json_result[i]["isBuyer"] << endl;
// 						if (json_result[i]["isBuyer"].asString() == "false") 
// 							Sells += atof(json_result[i]["quoteQty"].asString().c_str())*0.999;
// 						else
// 							Buys += atof(json_result[i]["quoteQty"].asString().c_str());
// 					}
// 				}
// 				else {
// 					cout << "json_result is not valid\n";
// 					cout << json_result << endl;
// 					return;
// 				}		
// 			}
//     		cout << endl;
//     		myfile.close();
//     	}
//     	else {
//     		cout << "myfile is not open\n";
//     		return;
//     	}
// 	}
// 	else {
// 		Nobitex::_GetMyTrades(str, PastDay);
// 		// cout << json_result << endl;

// 		if (isJsonResultValid(json_result)) {
// 			for (int i=0 ; i<json_result.size(); i++) {
// 				cout << "symbol: " << YELLOW(json_result[i]["symbol"]) << ", price: " << YELLOW(json_result[i]["price"]) << 
// 						", qty :" << json_result[i]["qty"] << ", quoteQty: " << json_result[i]["quoteQty"] << 
// 						", isBuyer: " << json_result[i]["isBuyer"] << endl;
// 				if (json_result[i]["isBuyer"].asString() == "false") 
// 					Sells += atof(json_result[i]["quoteQty"].asString().c_str())*0.999;
// 				else
// 					Buys += atof(json_result[i]["quoteQty"].asString().c_str());
// 			}
// 		}
// 		else {
// 			cout << "json_result is not valid\n";
// 			cout << json_result << endl;
// 			return;
// 		}		
// 	}




// 	// Nobitex::_GetMyTrades(str, PastDay);
// 	// // cout << json_result << endl;

// 	// if (isJsonResultValid(json_result)) {
// 	// 	for (int i=0 ; i<json_result.size(); i++) {
// 	// 		cout << "symbol: " << YELLOW(json_result[i]["symbol"]) << ", price: " << YELLOW(json_result[i]["price"]) << 
// 	// 				", qty :" << json_result[i]["qty"] << ", quoteQty: " << json_result[i]["quoteQty"] << 
// 	// 				", isBuyer: " << json_result[i]["isBuyer"] << endl;
// 	// 		if (json_result[i]["isBuyer"].asString() == "false") 
// 	// 			Sells += atof(json_result[i]["quoteQty"].asString().c_str())*0.999;
// 	// 		else
// 	// 			Buys += atof(json_result[i]["quoteQty"].asString().c_str());
// 	// 	}
// 	// }
// 	// else {
// 	// 	cout << "json_result is not valid\n";
// 	// 	cout << json_result << endl;
// 	// 	return;
// 	// }

// 	cout << "Considering all trades by " << YELLOW(str) << ". Buys " << Buys << " and sells " << Sells << ". ";
// 	if (Sells > Buys)
// 		cout << GREEN(Sells-Buys) << " ( " << GREEN((Sells-Buys)/Buys*100) << GREEN (" %") <<  " )\n";
// 	else
// 		cout << RED(Sells-Buys) << " ( " << RED((Sells-Buys)/Buys*100) << RED (" %") <<  " )\n";
// }

// void Nobitex::ShowDepositAddress(string &coin, string &network) {
// 	// cout << "ShowDepositAddress, coin: " << coin << ", network: " << network << "\n";

// 	string url;
// 	string baseAddress;

// 	baseAddress = "/sapi/v1/capital/deposit/address?";
	
// 	string temp;
// 	if (network != "isDefault")
// 		_pathQueryStringToUrl(url, baseAddress, temp, temp, coin, network, 0, 0, myRecvWindow, 0, 0, 0); 
// 	else 
// 		_pathQueryStringToUrl(url, baseAddress, temp, temp, coin, temp, 0, 0, myRecvWindow, 0, 0, 0); 
// 	// cout << "Inside _GetAccountInfoBalances, url: " << url << endl;

// 	string str_result;
// 	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
// 	// cout << "inside ShowDepositAddress, str_result: " << str_result << endl;

// 	Utility::ParseStringToJson("Nobitex::ShowDepositAddress", str_result, json_result);

// 	if (isJsonResultValid(json_result))
// 		cout << json_result << endl;
// 	else {
// 		cout << "json_result is not valid\n";
// 		cout << json_result << endl;
// 	}
// }

// void Nobitex::ShowDepositHistory(string str) {
// 	// cout << "ShowDepositHistory\n";

// 	string url;
// 	string baseAddress = "/wapi/v3/depositHistory.html?";

// 	string asset;
// 	if (str == "All") {}
// 	else 
// 		asset = str;

// 	string temp;
// 	_pathQueryStringToUrl(url, baseAddress, temp, asset, temp, temp, 0, 0, myRecvWindow, 0, 0, 0); 
// 	// cout << "url: " <<url << endl;	

// 	string str_result;
// 	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
// 	// cout << "inside ShowDepositHistory, str_result: " << str_result << endl;

// 	Utility::ParseStringToJson("Nobitex::ShowDepositHistory", str_result, json_result);

// 	if (isJsonResultValid(json_result))
// 		cout << json_result << endl;
// 	else {
// 		cout << "json_result is not valid\n";
// 		cout << json_result << endl;
// 	}
// }

// void Nobitex::ShowWithdrawHistory(string str) {
// 	// cout << "ShowWithdrawHistory\n";

// 	string url;
// 	string baseAddress = "/wapi/v3/withdrawHistory.html?";

// 	string asset;
// 	if (str == "All") {}
// 	else 
// 		asset = str;

// 	string temp;
// 	_pathQueryStringToUrl(url, baseAddress, temp, asset, temp, temp, 0, 0, myRecvWindow, 0, 0, 0); 
// 	// cout << "url: " <<url << endl;	

// 	string str_result;
// 	CurlAPI::CurlGetRequestWithMBXKey(url, str_result, api_key);
// 	// cout << "inside ShowDepositHistory, str_result: " << str_result << endl;

// 	Utility::ParseStringToJson("Nobitex::ShowWithdrawHistory", str_result, json_result);

// 	if (isJsonResultValid(json_result))
// 		cout << json_result << endl;
// 	else {
// 		cout << "json_result is not valid\n";
// 		cout << json_result << endl;
// 	}
// }

void Nobitex::SendOrder(string symbol, string side, string type, 
						double quantity, double price, double stopPrice, double stopLimitPrice) {
// 	cout << "symbol: " << symbol << ", side: " << side << ", type: " << type << ", quantity: " << quantity << ", price: " << price << ", stopPrice: " << stopPrice << ", stopLimitPrice: " << stopLimitPrice << endl << endl;

// 	if (Utility::AreYouSure("")) {
// 		string url;
// 		string baseAddress;
// 		if (type != "OCO")
// 			baseAddress = "/api/v3/order?";
// 		else 
// 			baseAddress = "/api/v3/order/oco?";


// 		string temp;
// 		if (type == "LIMIT" || type == "STOP_LOSS_LIMIT")
// 			_pathQueryStringToUrl_2(url, baseAddress, symbol, side, type, "GTC", quantity, price, 0, 0, temp, myRecvWindow);
// 		else if (type == "MARKET")
// 			_pathQueryStringToUrl_2(url, baseAddress, symbol, side, type, temp, quantity, 0, 0, 0, temp, myRecvWindow);
// 		else if (type == "STOP_LOSS")
// 			_pathQueryStringToUrl_2(url, baseAddress, symbol, side, type, temp, quantity, price, 0, 0, temp, myRecvWindow);
// 		else if (type == "OCO")
// 			_pathQueryStringToUrl_2(url, baseAddress, symbol, side, type, "GTC", quantity, price, stopPrice, stopLimitPrice, temp, myRecvWindow);
// 		else {
// 			cout << "Invalid type " << type << endl;
// 		}	
// 		cout << "url: " << url << endl;	

// 		string str_result;
// 		CurlAPI::CurlPostRequestWithMBXKey(url, str_result, api_key);
// 		// cout << "inside SendOrder, str_result: " << str_result << endl;

// 		Utility::ParseStringToJson("Nobitex::SendOrder", str_result, json_result);

// 		if (isJsonResultValid(json_result))
// 			cout << json_result << endl;
// 		else {
// 			cout << "json_result is not valid\n";
// 			cout << json_result << endl;
// 		}
// 		exit(0);
// 	}
}

void Nobitex::CancelOrder(string symbol, string orderId) {
// 	cout << "symbol: " << symbol << ", orderId: " << orderId << endl << endl;

// 	string url;
// 	string baseAddress = "/api/v3/order?";

// 	string temp;
// 	_pathQueryStringToUrl_2(url, baseAddress, symbol, temp, temp, temp, 0, 0, 0, 0, orderId, myRecvWindow);
// 	cout << "url: " << url << endl;

// 	string str_result;
// 	CurlAPI::CurlDeleteRequestWithMBXKey(url, str_result, api_key);
// 	// cout << "inside CancelOrder, str_result: " << str_result << endl;

// 	Utility::ParseStringToJson("Nobitex::CancelOrder", str_result, json_result);

// 	if (isJsonResultValid(json_result))
// 		cout << json_result << endl;
// 	else {
// 		cout << "json_result is not valid\n";
// 		cout << json_result << endl;
// 	}
}	

void Nobitex::CancelAllOrders(string symbol) {
	Utility::notSupported();
}



