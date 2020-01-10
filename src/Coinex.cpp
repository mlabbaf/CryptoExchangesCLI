#include "Coinex.h"
#include "CurlAPI.h"
#include "binacpp_utils.h"
#include "Utility.h"
#include "Log.h"

#include <openssl/md5.h>

using namespace std;

Coinex* Coinex::instance = 0;
Coinex* Coinex::getInstance() {
    if (instance == 0) {
        instance = new Coinex();
    }
    return instance;
}

static bool _IsJsonResultValid(Json::Value json_result) {
	if (json_result.isObject()) {
		if (json_result["code"].asInt64() == 0)
			return true;
		else {
			cout << json_result << endl;
			cout << RED ("Invalid code in json_result\n");
			return false;
		}
	}
	else {
		for (int i=0; i<json_result.size(); i++)
			if (json_result[i]["code"].asInt64() != 0) {
				cout << json_result[i] << endl;
				cout << RED ("Invalid code in json_result[" << RED (i) << "]");
				return false;
			}
		return true;
	}
} 

static void _createAuthorizationMd5(string &authorization, string secret_key, string querystring) {
	string temp = querystring + "&secret_key=" + secret_key;

	static const char hexDigits[17] = "0123456789ABCDEF";
	unsigned char digest[MD5_DIGEST_LENGTH];
	char digest_str[2*MD5_DIGEST_LENGTH+1];

	// Count digest
	MD5( (const unsigned char*)temp.c_str(), temp.length(), digest );

	// Convert the hash into a hex string form
	for(int i = 0; i < MD5_DIGEST_LENGTH; i++ ) {
        digest_str[i*2]   = hexDigits[(digest[i] >> 4) & 0xF];
        digest_str[i*2+1] = hexDigits[digest[i] & 0xF];
    }
    digest_str[MD5_DIGEST_LENGTH*2] = '\0';

    // std::cout <<"\nResult:"<< digest_str <<endl;
    authorization = digest_str;
}

int Coinex::_pathQueryStringToUrl(	string &url, string &authorization, string baseAddress, string coin_type,
									string market, long id, long page, long limit, string type, double amount,
									double price) {
	url = COINEX_HOST;
	url += baseAddress;

	string querystring(""); 

	querystring.append("access_id=");
	querystring.append(access_id);

	if ( amount > 0 ) {
		querystring.append("&amount=");
		querystring.append( to_string( amount ) );
	}

	if (coin_type.size() != 0) {
		querystring.append("&coin_type=");
		querystring.append(coin_type);
	}

	if ( id > 0 ) {
		querystring.append("&id=");
		querystring.append( to_string( id ) );
	}

	if ( limit > 0 ) {
		querystring.append("&limit=");
		querystring.append( to_string( limit ) );
	}

	if (market.size() != 0) {
		querystring.append("&market=");
		querystring.append(market);
	}

	if ( page > 0 ) {
		querystring.append("&page=");
		querystring.append( to_string( page ) );
	}

	if ( price > 0 ) {
		querystring.append("&price=");
		querystring.append( to_string( price ) );
	}

	querystring.append("&tonce=");
	querystring.append(to_string( get_current_ms_epoch()));

	if (type.size() != 0) {
		querystring.append("&type=");
		querystring.append(type);
	}

	url.append(querystring);

	_createAuthorizationMd5(authorization, secret_key, querystring);

	// cout << "querystring: " << querystring << endl;
	// cout << "url: " << url << endl;
	// cout << "authorization: " << authorization << endl;
	return 0;
}

int Coinex::_createJsonData(string &postData, string market, string type, double amount, double price) {

	postData = "{";
	
	postData.append("\"access_id\":");
	postData.append("\"" + access_id + "\", ");

	if (market.size() != 0) {
		postData.append("\"market\":");
		postData.append("\"" + market + "\", ");
	}

	if (type.size() != 0) {
		postData.append("\"type\":");
		postData.append("\"" + type + "\", ");
	}

	if (amount > 0) {
		postData.append("\"amount\":");
		postData.append("\"" + to_string(amount) + "\", ");
	}

	if (price > 0) {
		postData.append("\"price\":");
		postData.append("\"" + to_string(price) + "\", ");
	}

	postData.append("\"tonce\":");
	postData.append("\"" + to_string(get_current_ms_epoch()) + "\"");

	postData.append("}");

	// cout << "postData: " << postData << endl;
	return 0;
}

void Coinex::Init() {
	Exchange::setKeyFilePath("config/CoinexKeys.txt");
	Exchange::InitApiSecret();
}

void Coinex::Cleanup() {
	Exchange::Cleanup();
	// cout << "Successfully perform Coinex cleaning up\n";
}

void Coinex::_GetPriceBySymbol(string &symbol) {
	// cout << symbol << " - _GetPriceBySymbol\n";
	string url(COINEX_HOST);  
	transform(symbol.begin(), symbol.end(), symbol.begin(), ::toupper);
	url += "/market/ticker?market="+symbol;

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside _GetPriceBySymbol, str_result: " << str_result << endl;

	Utility::ParseStringToJson("_GetPriceBySymbol", str_result, json_result);
}

void Coinex::GetPrices(string &str, SymbolPriceSrtuct* result, int &len) {
	int counter = 1;

	if (str == "All") 
		cout << RED ("Coinex is not supporting All option\n");
	else if (str == "WatchList") {
		string line;
  		ifstream myfile ("config/WatchlistCoinex.txt");
  		if (myfile.is_open()) {
    		int cnt = 0;
    		while ( getline (myfile,line) ) {
    			Coinex::_GetPriceBySymbol(line);
    			if (_IsJsonResultValid(json_result)) {
					result[cnt].symbol = line;
					result[cnt++].price = atof(json_result["data"]["ticker"]["last"].asString().c_str());
				}
    		}
    		len = cnt;
    		myfile.close();
    	}
	}
	else {
		Coinex::_GetPriceBySymbol(str);
		if (_IsJsonResultValid(json_result)) {
			result[0].symbol = str;
			result[0].price = Utility::JsonToDouble(json_result["data"]["ticker"]["last"]);
			len = 1;
		}
	} 
}

void Coinex::ShowPrices(string str) {
	Exchange::ShowPrices(str);
}

void Coinex::_GetAccountInfoBalances() {
	string url, authorization;
	string baseAddress;

	baseAddress = "/balance/info?";

	string temp;
	_pathQueryStringToUrl(url, authorization, baseAddress, temp, temp, 0, 0, 0, temp, 0, 0);
	// cout << "Inside _GetAccountInfoBalances, url: " << url << ", authorization: " << authorization << endl;

	string str_result;
	// CurlAPI::CurlGetRequestWithMBXKey(url, str_result, access_id);
	CurlAPI::CurlGetRequestWithAuthorization(url, str_result, authorization);
	// cout << "inside _GetAccountInfoBalances, str_result: " << str_result << endl;

	Utility::ParseStringToJson("_GetAccountInfoBalances", str_result, json_result);
}

// mode is not used in Coinex exchange and is added for compatibility
map <string, map<string, double>> Coinex::GetBalances(_GetBalancesModes mode) {
	// cout << "GetBalances\n";

	Coinex::_GetAccountInfoBalances();
	
	map < string, map <string,double> >  userBalance;

	if (! _IsJsonResultValid(json_result))
		return userBalance;

	// for ( int i  = 0 ; i < json_result["data"].size() ; i++ ) {
	// 	// string symbol = json_result["balances"][i]["asset"].asString();
	// 	// userBalance[symbol]["f"] = atof( json_result["balances"][i]["free"].asString().c_str() );
	// 	// userBalance[symbol]["l"] = atof( json_result["balances"][i]["locked"].asString().c_str() );
	// 	cout << json_result["data"][i] << endl;
	// 	cout << json_result["data"][i].asString() << endl;
	// }
	// cout << fastWriter.write(json_result["data"]) << endl;

	Json::Value::Members members = json_result["data"].getMemberNames();
	for (vector<string>::iterator iter = members.begin(); iter != members.end(); iter++) {
		string symbol = *iter;
		userBalance[symbol]["f"] = atof( json_result["data"][*iter]["available"].asString().c_str() );
		userBalance[symbol]["l"] = atof( json_result["data"][*iter]["frozen"].asString().c_str() );

		// cout << *iter << " - " << json_result["data"][*iter] << endl;
	}

	return userBalance;
}

void Coinex::ShowBalances() {
	Exchange::ShowBalances();
}

map <string, StructBalanceInUSDT> Coinex::ShowBalanceInUSDT() {
	map <string, StructBalanceInUSDT> balanceResult;

	// Get balance
	map <string, map<string,double>> userBalance;
	userBalance = GetBalances(BANK_AND_EXCHANGE_MODE);

	// Get current prices
	string str = "WatchList";
	SymbolPriceSrtuct result[50];
	int len;
	Coinex::GetPrices(str, result, len);

	// Calculate whole balances
	double wholeBalance = 0;
	for (map <string, map<string, double>>::iterator it_i=userBalance.begin(); it_i!= userBalance.end(); it_i++) {
		string symbol 			= (*it_i).first;
		map <string,double> balance 	= (*it_i).second;
		double coinBalance = balance["available"] + balance["frozen"];

		if (coinBalance > 0) {
			// Get price
			double price = -1;
			if (symbol == "USDT") 
				price = 1;
			else
				for (int i=0; i<len; i++)
					if (result[i].symbol == (symbol+"USDT"))
						price = result[i].price;
			if (price == -1) {
				cout << YELLOW("Warning:") << " cannot find price for " << symbol << endl;
				price = 0;
				// return;
			}

			wholeBalance += coinBalance * price;
			cout << symbol << ": " << YELLOW(coinBalance) << " (" << balance["available"] << " , " 
				 << balance["frozen"] << ") at price " << YELLOW(price) 
				 << " (" << RED(coinBalance*price) << ")\n";
			
			balanceResult[symbol].balance = coinBalance;
			balanceResult[symbol].balanceFree = balance["available"];
			balanceResult[symbol].balanceLocked = balance["frozen"];
			balanceResult[symbol].price = price;
			balanceResult[symbol].asset = coinBalance*price;
		}
	}

	cout << "\nwholeBalance is " << YELLOW(wholeBalance) << " USDT\n\n";

	return balanceResult;
}

bool Coinex::_GetOpenOrdersForOneSymbol(string symbol, vector<SymbolOrderStruct> &vecOpenOrders) {
	string url, authorization;
	string baseAddress;

	baseAddress = "/order/pending?";

	string temp;
	_pathQueryStringToUrl(url, authorization, baseAddress, temp, symbol, 0, 1, 100, temp, 0, 0);
	// cout << "Inside GetOpenOrders, url: " << url << endl;

	string str_result;
	CurlAPI::CurlGetRequestWithAuthorization(url, str_result, authorization);
	// cout << "inside _GetOpenOrdersForOneSymbol, str_result: " << str_result << endl;

	Json::Value jsonOpenOrders;
	Utility::ParseStringToJson("_GetOpenOrdersForOneSymbol", str_result, jsonOpenOrders);

	if (_IsJsonResultValid(jsonOpenOrders)) {
		// cout << jsonOpenOrders << endl;
		for (int i=0; i<jsonOpenOrders["data"]["count"].asInt64(); i++) {
			Json::Value json_temp = jsonOpenOrders["data"]["data"][i];
			
			SymbolOrderStruct order;
			order.symbol = symbol;
			order.price = Utility::JsonToDouble(json_temp["price"]);
			order.type = json_temp["type"].asString();
			order.amount = Utility::JsonToDouble(json_temp["amount"]);
			order.left = Utility::JsonToDouble(json_temp["left"]);
			order.order_type = json_temp["order_type"].asString();
			order.id = Utility::JsonToLong(json_temp["id"]);

			vecOpenOrders.push_back(order);
		}
	}
	else
		return false;
	return true;
}

bool Coinex::GetOpenOrders(string &str, vector <SymbolOrderStruct> &vecOpenOrders) {
	if (str == "All") {
		// map <string, map<string,double>> userBalance;
		// userBalance = GetBalances(BANK_AND_EXCHANGE_MODE);
		
		// map < string, map<string,double> >::iterator iter;
		// for (iter = userBalance.begin(); iter != userBalance.end(); iter++) {
		// 	string symbol = (*iter).first;
		// 	map <string,double> balance = (*iter).second;
		// 	if ((balance["available"] != 0 || balance["frozen"] != 0) && symbol != "USDT") {
		// 		if (_GetOpenOrdersForOneSymbol(symbol+"USDT", vecOpenOrders)) {}
		// 		else 
		// 			return false;
		// 	}
		// }
		cout << "GetOpenOrders with All option cannot be implemented for Coinex\n";
		return false;
	}
	else if (str == "WatchList") {
		ifstream myfile ("config/WatchlistCoinex.txt");
		string line;
		if (myfile.is_open()) {
			while ( getline (myfile, line) ) {
				if (_GetOpenOrdersForOneSymbol(line, vecOpenOrders)) {}
				else {	
					myfile.close();
					return false;
				}
			}
			myfile.close();
		}
		else {
			cout << "Cannot open WatchlistCoinex\n";
			return false;
		}
	}
	else
		return (_GetOpenOrdersForOneSymbol(str, vecOpenOrders));

	return true;
}

void Coinex::ShowOpenOrders(string str) {
	// cout << "ShowOpenOrders\n";

	vector <SymbolOrderStruct> vecOpenOrders;
	if (Coinex::GetOpenOrders(str, vecOpenOrders))
		for (vector <SymbolOrderStruct>::iterator iter = vecOpenOrders.begin(); iter != vecOpenOrders.end(); iter ++)
			cout << "symbol: " << YELLOW(iter->symbol) << ", price: " << YELLOW(iter->price) <<
					", type: " << iter->type << ", amount: " << iter->amount <<
					", left: " << iter->left << ", order_type: " << iter->order_type << 
					", id: " << iter->id << endl;					
	else 
		cout << "Cannot GetOpenOrders\n";
}

void Coinex::_GetMyTrades(string &str, int limit) {
	string url, authorization;
	string baseAddress = "/order/user/deals?";

	string temp;
	_pathQueryStringToUrl(url, authorization, baseAddress, temp, str, 0, 1, limit, temp, 0, 0);
	// cout << "Inside _GetMyTrades, url: " << url << endl;

	string str_result;
	CurlAPI::CurlGetRequestWithAuthorization(url, str_result, authorization);
	// cout << "inside _GetMyTrades, str_result: " << str_result << endl;

	Utility::ParseStringToJson("_GetMyTrades", str_result, json_result);
}

void Coinex::ShowMyTrades(string str, int pastDay) {
	// cout << "ShowMyTrades\n";

	struct timeval tv;
	gettimeofday(&tv, NULL); 

	int limit;
	if (pastDay == 0)
		limit = 100;
	else
		limit = 20;

	if (str == "WatchList") {
		// Json::Value::const_iterator iter;
		string line;
  		ifstream myfile ("config/WatchlistCoinex.txt");
  		if (myfile.is_open()) {
    		while ( getline (myfile, line) ) {
				Coinex::_GetMyTrades(line, limit);
				if (_IsJsonResultValid(json_result)) {
					// cout << json_result << endl;
					for (int i=0; i<json_result["data"]["count"].asInt64(); i++) {
						Json::Value json_temp = json_result["data"]["data"][i];
						if (pastDay == 0 || (tv.tv_sec - pastDay*24*60*60) < Utility::JsonToLong(json_temp["create_time"])) {
							cout << "symbol: " << YELLOW(line) << ", price: " << YELLOW(json_temp["price"]) << 
									", amount:" << json_temp["amount"] << ", type: " << json_temp["type"] << endl;
						}
					}
				}
				else {
					cout << "Invalid json_result in ShowMyTrades\n";
					cout << json_result << endl;
					return;
				}
    		}
    		cout << endl;
    		myfile.close();
    	}
	}
	else {
		Coinex::_GetMyTrades(str, limit);
		if (_IsJsonResultValid(json_result)) {
			// cout << json_result << endl;
			for (int i=0; i<json_result["data"]["count"].asInt64(); i++) {
				Json::Value json_temp = json_result["data"]["data"][i];
				if (pastDay == 0 || (tv.tv_sec - pastDay*24*60*60) < Utility::JsonToLong(json_temp["create_time"])) {
					cout << "symbol: " << YELLOW(str) << ", price: " << YELLOW(json_temp["price"]) << 
							", amount:" << json_temp["amount"] << ", type: " << json_temp["type"] << endl;
				}
			}
		}
		else {
			cout << "Invalid json_result in ShowMyTrades\n";
			cout << json_result << endl;
		}
	}
}

void Coinex::ShowTradesPerformance(string &str, int pastDay) {
	// cout << "ShowTradesPerformance\n";
	struct timeval tv;
	gettimeofday(&tv, NULL); 
	
	int limit;
	if (pastDay == 0)
		limit = 100;
	else
		limit = 20;
	limit = limit*5;


	double Sells = 0, Buys = 0;

	if (str == "WatchList") {
		// Json::Value::const_iterator iter;
		string line;
  		ifstream myfile ("config/WatchlistCoinex.txt");
  		if (myfile.is_open()) {
    		while ( getline (myfile, line) ) {
				Coinex::_GetMyTrades(line, limit);
				// cout << json_result << endl;

				if (_IsJsonResultValid(json_result)) {
					for (int i=0; i<json_result["data"]["count"].asInt64(); i++) {
						Json::Value json_temp = json_result["data"]["data"][i];
						if (pastDay == 0 || (tv.tv_sec - pastDay*24*60*60) < Utility::JsonToLong(json_temp["create_time"])) {
							cout << "symbol: " << YELLOW(str) << ", price: " << YELLOW(json_temp["price"]) << 
									", amount:" << json_temp["amount"] << ", type: " << json_temp["type"] << endl;
							if (json_temp["type"].asString() == "sell") 
								Sells += Utility::JsonToDouble(json_temp["amount"])*Utility::JsonToDouble(json_temp["price"])*0.999;
							else
								Buys += Utility::JsonToDouble(json_temp["amount"])*Utility::JsonToDouble(json_temp["price"]);
						}
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
	}
	else {
		Coinex::_GetMyTrades(str, limit);
		// cout << json_result << endl;

		if (_IsJsonResultValid(json_result)) {
			for (int i=0; i<json_result["data"]["count"].asInt64(); i++) {
				Json::Value json_temp = json_result["data"]["data"][i];
				if (pastDay == 0 || (tv.tv_sec - pastDay*24*60*60) < Utility::JsonToLong(json_temp["create_time"])) {
					cout << "symbol: " << YELLOW(str) << ", price: " << YELLOW(json_temp["price"]) << 
							", amount:" << json_temp["amount"] << ", type: " << json_temp["type"] << endl;
					if (json_temp["type"].asString() == "sell") 
						Sells += Utility::JsonToDouble(json_temp["amount"])*Utility::JsonToDouble(json_temp["price"])*0.999;
					else
						Buys += Utility::JsonToDouble(json_temp["amount"])*Utility::JsonToDouble(json_temp["price"]);
				}
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

void Coinex::ShowDepositHistory(string str) {
	// cout << "ShowDepositHistory\n";

	string url, authorization;
	string baseAddress = "/balance/coin/deposit?";

	string coin_type;
	if (str == "All") {}
	else 
		coin_type = str;

	string temp;
	_pathQueryStringToUrl(url, authorization, baseAddress, coin_type, temp, 0, 0, 0, temp, 0, 0);
	// cout << "url: " <<url << endl;	

	string str_result;
	CurlAPI::CurlGetRequestWithAuthorization(url, str_result, authorization);
	// cout << "inside ShowDepositHistory, str_result: " << str_result << endl;

	Utility::ParseStringToJson("ShowDepositHistory", str_result, json_result);

	cout << json_result << endl;	
}

void Coinex::ShowWithdrawHistory(string str) {
	// cout << "ShowWithdrawHistory\n";

	string url, authorization;
	string baseAddress = "/balance/coin/withdraw?";

	string coin_type;
	if (str == "All") {}
	else 
		coin_type = str;

	string temp;
	_pathQueryStringToUrl(url, authorization, baseAddress, coin_type, temp, 0, 0, 0, temp, 0, 0);
	// cout << "url: " <<url << endl;	

	string str_result;
	CurlAPI::CurlGetRequestWithAuthorization(url, str_result, authorization);
	// cout << "inside ShowDepositHistory, str_result: " << str_result << endl;

	Utility::ParseStringToJson("ShowWithdrawHistory", str_result, json_result);

	cout << json_result << endl;	
}

void Coinex::SendOrder(string symbol, string side, string type, double quantity, double price) {
	cout << "symbol: " << symbol << ", side: " << side << ", type: " << type << ", quantity: " << quantity << ", price: " << price << endl << endl;

	if (Utility::AreYouSure("")) {
		string url(COINEX_HOST);  

		string authorization;
		if (type == "LIMIT")
			url += "/order/limit";
		else if (type == "MARKET")
			url += "/order/market";
		else {
			cout << RED ("Invalid type " + type + "\n");
			return;
		}

		string url_temp, temp;
		if (type == "LIMIT")
			Coinex::_pathQueryStringToUrl(url_temp, authorization, temp, temp, symbol, 0, 0, 0, side, quantity, price);
		else if (type == "MARKET")
			Coinex::_pathQueryStringToUrl(url_temp, authorization, temp, temp, symbol, 0, 0, 0, side, quantity, 0);
		else {
			cout << RED ("Invalid type " + type + "\n");
			return;
		}
		// cout << "url_temp: " <<  url_temp << endl;

		string postData;
		if (type == "LIMIT")
			Coinex::_createJsonData(postData, symbol, side, quantity, price);
		else if (type == "MARKET")
			Coinex::_createJsonData(postData, symbol, side, quantity, 0);
		else {
			cout << RED ("Invalid type " + type + "\n");
			return;
		}
		cout << "url: " << url << ", postData: " << postData << ", authorization: " << authorization << endl;

		string str_result;
		CurlAPI::CurlPostRequestWithAuthorization(url, str_result, authorization, postData);
		// cout << "inside SendOrder, str_result: " << str_result << endl;

		Utility::ParseStringToJson("SendOrder", str_result, json_result);

		cout << json_result << endl;	
		exit(0);
	}
}

void Coinex::CancelOrder(string symbol, long orderId) {
	cout << "symbol: " << symbol << ", orderId: " << orderId << endl << endl;

	string url, authorization;
	string baseAddress = "/order/pending?";

	string temp;
	_pathQueryStringToUrl(url, authorization, baseAddress, temp, symbol, orderId, 0, 0, temp, 0, 0);
	// cout << "url: " << url << endl;

	string str_result;
	CurlAPI::CurlDeleteRequestWithAuthorization(url, str_result, authorization);
	// cout << "inside CancelOrder, str_result: " << str_result << endl;

	Utility::ParseStringToJson("CancelOrder", str_result, json_result);

	cout << json_result << endl;	
}	

