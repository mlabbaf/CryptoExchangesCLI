#include "HitBTC.h"
#include "CurlAPI.h"
#include "binacpp_utils.h"


using namespace std;

CURL* HitBTC::curl = NULL;
Json::Value HitBTC::json_result;
Json::FastWriter HitBTC::fastWriter;
string HitBTC::secret_key;
string HitBTC::api_key;


static bool _IsJsonResultValid(Json::Value json_result) {
	if (json_result.isObject()) {
		if (json_result.isMember("error"))
			return false;
	}

	return true;
} 

// symbol is necessary
int HitBTC::_pathQueryStringToUrl(	string &url, string baseAddress, string symbol, string from, string until,
									string side, string type, string id, string by) {
	url = HITBTC_HOST;
	url += baseAddress;

	string querystring(""); 

	// if (symbol.size() != 0) {
		querystring.append("symbol=");
		querystring.append(symbol);
	// }

	if ( from.size() != 0 ) {
		querystring.append("&from=");
		querystring.append(from);
	}

	if ( until.size() != 0 ) {
		querystring.append("&until=");
		querystring.append(until);
	}

	if (side.size() != 0) {
		querystring.append("&side=");
		querystring.append(side);
	}

	if (type.size() != 0) {
		querystring.append("&type=");
		querystring.append(type);
	}

	if (id.size() != 0) {
		querystring.append("&id=");
		querystring.append(id);
	}

	if (by.size() != 0) {
		querystring.append("&by=");
		querystring.append(by);
	}

	url.append(querystring);

	// cout << "url: " << url << endl;
	return 0;
}

int HitBTC::_createJsonData(string &postData, string symbol, string currency, string side, 
							string type, double quantity, double amount, double price, double stopPrice) {

	postData = "{";
	
	if (symbol.size() != 0) {
		postData.append("\"symbol\":");
		postData.append("\"" + symbol + "\", ");
	}

	if (currency.size() != 0) {
		postData.append("\"currency\":");
		postData.append("\"" + currency + "\", ");
	}

	if (side.size() != 0) {
		transform(side.begin(), side.end(), side.begin(), ::tolower);
		postData.append("\"side\":");
		postData.append("\"" + side + "\", ");
	}

	if (type.size() != 0) {
		postData.append("\"type\":");
		postData.append("\"" + type + "\", ");
	}

	if (quantity > 0) {
		postData.append("\"quantity\":");
		postData.append("\"" + to_string(quantity) + "\", ");
	}

	if (amount > 0) {
		postData.append("\"amount\":");
		postData.append("\"" + to_string(amount) + "\", ");
	}

	if (price > 0) {
		postData.append("\"price\":");
		postData.append("\"" + to_string(price) + "\", ");
	}

	if (stopPrice > 0) {
		postData.append("\"stopPrice\":");
		postData.append("\"" + to_string(stopPrice) + "\", ");
	}

	postData.pop_back();
	postData.pop_back();
	postData.append("}");

	cout << "postData: " << postData << endl;
	return 0;
}

static void _GetStartTimeFromDay(int pastDay, string &startTime, string &endTime) {
	if (pastDay == 0) 	// All times
		startTime = "";
	else {
		time_t now;
	    time(&now);
	    char buf[sizeof "2011-10-08T07:07:09.Z"];
	    struct tm* tm = gmtime(&now);

	    tm->tm_mday -= pastDay;
	    now = mktime(tm);

	    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
	    // std::cout << buf << "\n";
	    startTime = buf;
	}
}


void HitBTC::Init() {
	string line; 
	ifstream myfile ("config/HitBTCKeys.txt");
	if (myfile.is_open()) {
		getline (myfile, api_key);
		getline (myfile, secret_key);
			
		// cout << api_key << endl;
		// cout << secret_key << endl;

		myfile.close();
	}
	else {
		cout << RED("Cannot open HitBTCKeys.txt\n");
		exit(-1);
	}
}

void HitBTC::Cleanup() {
	// cout << "Successfully perform HitBTC cleaning up\n";
}

void HitBTC::_GetAllPrices() {
	string url(HITBTC_HOST);  
	url += "/public/ticker";

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside _GetAllPrices, str_result: " << str_result << endl;

	Utility::ParseStringToJson("_GetAllPrices", str_result, json_result);
}

void HitBTC::_GetPriceBySymbol(string &symbol) {
	// cout << symbol << " - _GetPriceBySymbol\n";
	string url(HITBTC_HOST);  
	url += "/public/ticker/"+symbol;

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside _GetPriceBySymbol, str_result: " << str_result << endl;

	Utility::ParseStringToJson("_GetPriceBySymbol", str_result, json_result);
}

void HitBTC::GetPrices(string &str, SymbolPriceSrtuct* result, int &len) {

	if (str == "All") {
		HitBTC::_GetAllPrices();
		if (_IsJsonResultValid(json_result)) {
			int cnt = 0;
			for (Json::Value::const_iterator iter = json_result.begin(); iter != json_result.end(); iter++) {
				result[cnt].symbol = (*iter)["symbol"].asString();
				result[cnt].price = Utility::JsonToDouble((*iter)["last"]);
				cnt++;
			}
			len = cnt;
		}
	}
	else if (str == "WatchList") {
		HitBTC::_GetAllPrices();
		if (_IsJsonResultValid(json_result)) {
			string line;
	  		ifstream myfile ("config/WatchlistHitBTC.txt");
	  		if (myfile.is_open()) {
	    		int cnt = 0;
	    		while ( getline (myfile, line) ) {
					for (Json::Value::const_iterator iter = json_result.begin(); iter != json_result.end(); iter++) {
						if ((*iter)["symbol"] == line) {
							result[cnt].symbol = (*iter)["symbol"].asString();
							result[cnt].price = Utility::JsonToDouble((*iter)["last"]);
							cnt++;
						}
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
		HitBTC::_GetPriceBySymbol(str);
		if (_IsJsonResultValid(json_result)) {
			result[0].symbol = json_result["symbol"].asString();
			result[0].price = Utility::JsonToDouble(json_result["last"]);
			len = 1;
		}
	} 
}

void HitBTC::ShowPrices(string str) {
	// cout << "Inside ShowPrices\n";
	
	SymbolPriceSrtuct result[2000];
	int len;
	HitBTC::GetPrices(str, result, len);

	for (int i=0; i<len; i++)
		cout << "symbol: " << result[i].symbol << ", last_price: " << YELLOW(result[i].price) << endl;
}

void HitBTC::_GetAccountInfoBalances() {
	string url(HITBTC_HOST);  
	url += "/account/balance";

	string str_result;
	CurlAPI::CurlGetRequestWithBasicAuthentication(url, str_result, api_key, secret_key);
	// cout << "inside _GetAccountInfoBalances, str_result: " << str_result << endl;

	Utility::ParseStringToJson("_GetAccountInfoBalances", str_result, json_result);

	if (_IsJsonResultValid(json_result)) {}
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void HitBTC::_GetTradingBalances() {
	string url(HITBTC_HOST);  
	url += "/trading/balance";

	string str_result;
	CurlAPI::CurlGetRequestWithBasicAuthentication(url, str_result, api_key, secret_key);
	// cout << "inside _GetTradingBalances, str_result: " << str_result << endl;

	Utility::ParseStringToJson("_GetTradingBalances", str_result, json_result);

	if (_IsJsonResultValid(json_result)) {}
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

map <string, map<string, double>> HitBTC::GetBalances(_GetBalancesModes mode) {
	// cout << "GetBalances\n";

	map < string, map <string,double> >  userBalance;
	
	if (mode == BANK_AND_EXCHANGE_MODE || mode == ONLY_BANK_MODE) {
		HitBTC::_GetAccountInfoBalances();

		if (_IsJsonResultValid(json_result)) {
			for (Json::Value::const_iterator json_iter = json_result.begin(); json_iter != json_result.end(); json_iter++) {
				string symbol = (*json_iter)["currency"].asString();
				userBalance[symbol]["f"] = Utility::JsonToDouble((*json_iter)["available"]);
				userBalance[symbol]["l"] = Utility::JsonToDouble((*json_iter)["reserved"]);
			}
		}
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
		}
	}

	if (mode == BANK_AND_EXCHANGE_MODE || mode == ONLY_EXCHANGE_MODE) {
		HitBTC::_GetTradingBalances();
		
		if (_IsJsonResultValid(json_result)) {
			for (Json::Value::const_iterator json_iter = json_result.begin(); json_iter != json_result.end(); json_iter++) {
				string symbol = (*json_iter)["currency"].asString();
				userBalance[symbol]["f"] += Utility::JsonToDouble((*json_iter)["available"]);
				userBalance[symbol]["l"] += Utility::JsonToDouble((*json_iter)["reserved"]);
			}
		}
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
		}
	}

	return userBalance;
}

void HitBTC::ShowBalances() {
	// cout << "ShowBalances\n";
	
	map <string, map<string,double>> userBalance;
	userBalance = GetBalances(BANK_AND_EXCHANGE_MODE);

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

void HitBTC::ShowBankBalances() {
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

void HitBTC::ShowExchangeBalances() {
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

map <string, StructBalanceInUSDT> HitBTC::ShowBalanceInUSDT() {
	map <string, StructBalanceInUSDT> balanceResult;

	// Get balance
	map <string, map<string,double>> userBalance;
	userBalance = GetBalances(BANK_AND_EXCHANGE_MODE);

	// Get current prices
	string str = "All";
	SymbolPriceSrtuct result[2000];
	int len;
	HitBTC::GetPrices(str, result, len);


	// Calculate whole balances
	double wholeBalance = 0;
	for (map <string, map<string, double>>::iterator it_i=userBalance.begin(); it_i!= userBalance.end(); it_i++) {
		string symbol 			= (*it_i).first;
		map <string,double> balance 	= (*it_i).second;
		double coinBalance = balance["f"] + balance["l"];

		if (coinBalance > 0) {
			// Get price
			double price = -1;
			if (symbol == "USD") 
				price = 1;
			else
				for (int i=0; i<len; i++) {
					string pairCoin;
					if (symbol == "XRP")
						pairCoin = "USDT";
					else
						pairCoin = "USD";

					if (result[i].symbol == (symbol+pairCoin))
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
		
			if (symbol == "USD")
				symbol = "USDT";
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

bool HitBTC::GetOpenOrders(string &str, Json::Value &jsonOpenOrders) {
	string url(HITBTC_HOST);  
	url += "/order";

	if (str != "All" && str != "WatchList")
		url = url + "?symbol=" + str;

	string str_result;
	CurlAPI::CurlGetRequestWithBasicAuthentication(url, str_result, api_key, secret_key);
	// cout << "inside GetOpenOrders, str_result: " << str_result << endl;

	Utility::ParseStringToJson("HitBTC::GetOpenOrders", str_result, jsonOpenOrders);
	
	if (_IsJsonResultValid(jsonOpenOrders)) 
		return true;
	else {
		cout << "jsonOpenOrders is not valid\n";
		cout << jsonOpenOrders << endl;
		return false;
	}
}

void HitBTC::ShowOpenOrders(string str) {
	// cout << "ShowOpenOrders\n";
	
	if (HitBTC::GetOpenOrders(str, json_result)) {
		for (int i=0; i<json_result.size(); i++) {
				cout << "symbol: " << YELLOW(json_result[i]["symbol"]) << ", price: " << YELLOW(json_result[i]["price"]) <<
						", side: " << json_result[i]["side"] <<	", type: " << json_result[i]["type"] << 
						", qnt: " << json_result[i]["quantity"] << ", cum: " << json_result[i]["cumQuantity"] <<
						", clientOrderId: " << json_result[i]["clientOrderId"] << endl;
						// ", id: " << json_result[i]["id"] << endl;
						// ", id: " << json_result[i]["id"] << ", clientOrderId: " << json_result[i]["clientOrderId"] << endl;
		}
	}
	else
		cout << "Cannot GetOpenOrders\n";
}

void HitBTC::_GetMyTrades(string &str, int PastDay) {
	string url;
	string baseAddress = "/history/trades?";

	string startTime, endTime;
	_GetStartTimeFromDay(PastDay, startTime, endTime);

	string temp; 
	if (str != "All" && str != "WatchList")
		_pathQueryStringToUrl(url, baseAddress, str, startTime, endTime, temp, temp, temp, "timestamp");
	else
		_pathQueryStringToUrl(url, baseAddress, temp, startTime, endTime, temp, temp, temp, "timestamp");
	// cout << url << endl;

	string str_result;
	CurlAPI::CurlGetRequestWithBasicAuthentication(url, str_result, api_key, secret_key);
	// cout << "inside _GetMyTrades, str_result: " << str_result << endl;

	Utility::ParseStringToJson("HitBTC::_GetMyTrades", str_result, json_result);
	
	if (_IsJsonResultValid(json_result)) {}
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void HitBTC::ShowMyTrades(string str, int PastDay) {
	// cout << "ShowMyTrades\n";

	if (str == "WatchList") {
		Json::Value::const_iterator iter;
		string line;
  		ifstream myfile ("config/WatchlistHitBTC.txt");
  		if (myfile.is_open()) {
    		while ( getline (myfile, line) ) {
				HitBTC::_GetMyTrades(line, PastDay);
				if (_IsJsonResultValid(json_result))
					for (int i=0 ; i<json_result.size(); i++)
						cout << "symbol: " << YELLOW(json_result[i]["symbol"]) << ", price: " << YELLOW(json_result[i]["price"]) << 
								", quantity :" << json_result[i]["quantity"] << ", side: " << json_result[i]["side"] << endl;
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
		HitBTC::_GetMyTrades(str, PastDay);
		if (_IsJsonResultValid(json_result))
			for (int i=0 ; i<json_result.size(); i++)
				cout << "symbol: " << YELLOW(json_result[i]["symbol"]) << ", price: " << YELLOW(json_result[i]["price"]) << 
						", quantity :" << json_result[i]["quantity"] << ", side: " << json_result[i]["side"] << endl;
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
		}
	}
}

void HitBTC::ShowTradesPerformance(string &str, int PastDay) {
	// cout << "ShowTradesPerformance\n";

	HitBTC::_GetMyTrades(str, PastDay);
	// cout << json_result << endl;

	double Sells = 0, Buys = 0;
	// if (json_result.isArray()) {
	if (_IsJsonResultValid(json_result)) {
		for (int i=0 ; i<json_result.size(); i++) {
			cout << "symbol: " << YELLOW(json_result[i]["symbol"]) << ", price: " << YELLOW(json_result[i]["price"]) << 
					", quantity :" << json_result[i]["quantity"] << ", side: " << json_result[i]["side"] << endl;
			if (json_result[i]["side"].asString() == "sell") 
				Sells += Utility::JsonToDouble(json_result[i]["quantity"])*Utility::JsonToDouble(json_result[i]["price"])*0.999;
			else
				Buys += Utility::JsonToDouble(json_result[i]["quantity"])*Utility::JsonToDouble(json_result[i]["price"]);
		}
	}
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
		return;
	}

	cout << "Considering all trades by " << YELLOW(str) << ". Buys " << Buys << " and sells " << Sells << ". ";
	if (Sells > Buys)
		cout << GREEN(Sells-Buys) << " ( " << GREEN((Sells-Buys)/Buys*100) << GREEN (" %") <<  " )\n";
	else
		cout << RED(Sells-Buys) << " ( " << RED((Sells-Buys)/Buys*100) << RED (" %") <<  " )\n";
}

void HitBTC::ShowDepositAddress(string &str) {
	// cout << "ShowDepositAddress\n";

	string url(HITBTC_HOST);  
	url += "/account/crypto/address/";
	url += str;

	string str_result;
	CurlAPI::CurlGetRequestWithBasicAuthentication(url, str_result, api_key, secret_key);
	// cout << "inside ShowDepositAddress, str_result: " << str_result << endl;

	Utility::ParseStringToJson("ShowDepositAddress", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}

void HitBTC::SendOrder(string symbol, string side, string type, double quantity, double price) {
	cout << "symbol: " << symbol << ", side: " << side << ", type: " << type << ", quantity: " << quantity << ", price: " << price << endl << endl;

	if (Utility::AreYouSure("")) {
		string url(HITBTC_HOST);  
		url += "/order";

		string postData;
		string temp;
		if (type == "limit")
			HitBTC::_createJsonData(postData, symbol, temp, side, type, quantity, 0, price, 0);
		else if (type == "market")
			HitBTC::_createJsonData(postData, symbol, temp, side, type, quantity, 0, 0, 0);
		else if (type == "stopLimit") {
			double sellPrice = price * 0.99;
			Utility::RoundPriceBasedOnPair(symbol, sellPrice);
			HitBTC::_createJsonData(postData, symbol, temp, side, type, quantity, 0, sellPrice, price);
		}
		else {
			cout << RED ("Invalid type " + type + "\n");
			return;
		}

		string str_result;
		CurlAPI::CurlPostRequestWithBasicAuthentication(url, str_result, api_key, secret_key, postData);
		// cout << "inside SendOrder, str_result: " << str_result << endl;

		Utility::ParseStringToJson("SendOrder", str_result, json_result);
		
		if (_IsJsonResultValid(json_result))
			cout << json_result << endl;
		else {
			cout << "json_result is not valid\n";
			cout << json_result << endl;
		}
		exit(0);
	}
}

void HitBTC::CancelOrder(string symbol, string clientOrderId) {
	cout << "symbol: " << symbol << ", clientOrderId: " << clientOrderId << endl << endl;

	string url(HITBTC_HOST);  
	url += "/order/";
	url += clientOrderId;
	// cout << "url: " << url << endl;	

	string str_result;
	CurlAPI::CurlDeleteRequestWithBasicAuthentication(url, str_result, api_key, secret_key);
	// cout << "inside CancelOrder, str_result: " << str_result << endl;

	Utility::ParseStringToJson("CancelOrder", str_result, json_result);

	if (_IsJsonResultValid(json_result))
		cout << json_result << endl;
	else {
		cout << "json_result is not valid\n";
		cout << json_result << endl;
	}
}	

void HitBTC::TransferBetweenBankAndExchange(string currency, double amount, string type) {
	cout << "currency: " << currency << ", type: " << type << ", amount: " << amount << endl;

	string url(HITBTC_HOST);  
	url += "/account/transfer";

	string postData;
	string temp;
	HitBTC::_createJsonData(postData, temp, currency, temp, type, 0, amount, 0, 0);

	string str_result;
	CurlAPI::CurlPostRequestWithBasicAuthentication(url, str_result, api_key, secret_key, postData);
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
