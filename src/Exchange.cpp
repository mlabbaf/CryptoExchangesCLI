#include "Exchange.h"
#include "CurlAPI.h"
#include "binacpp_utils.h"


using namespace std;


void Exchange::Init() {
	this->InitApiSecret();
}

void Exchange::Cleanup() {
	// cout << "Successfully perform Exchange cleaning up\n";
}

void Exchange::setKeyFilePath(string exchangeKeyFilePath) {
	keyFilePath = exchangeKeyFilePath;
}

void Exchange::setWatchlistPath(string exchangeWatchlistPath) {
	watchlistPath = exchangeWatchlistPath;
}

void Exchange::InitApiSecret() {
	string line; 
	ifstream myfile (keyFilePath);
	if (myfile.is_open()) {
		getline (myfile, api_key);
		access_id = api_key;
		getline (myfile, secret_key);
			
		// cout << api_key << endl;
		// cout << secret_key << endl;

		myfile.close();
	}
	else {
		cout << RED("Cannot open " + keyFilePath + "\n");
		exit(-1);
	}
}

void Exchange::InitApiSecretPassphrase() {
	string line; 
	ifstream myfile (keyFilePath);
	if (myfile.is_open()) {
		getline (myfile, api_key);
		getline (myfile, secret_key);
		getline (myfile, passphrase);
			
		// cout << api_key << endl;
		// cout << secret_key << endl;
		// cout << passphrase << endl;

		myfile.close();
	}
	else {
		cout << RED("Cannot open " + keyFilePath + "\n");
		exit(-1);
	}
}

void Exchange::GetPrices(string &str, SymbolPriceSrtuct* result, int &len) {
	if (str == "All")
		this->getAllPrices(str, result, len);
	else if (str == "WatchList")
		this->getWatchlistPrices(str, result, len);
	else 
		getSymbolPrice(str, result, len);
}

void Exchange::ShowPrices(string str) {
	// cout << "Inside ShowPrices\n";

	SymbolPriceSrtuct result[10000];
	int len;
	this->GetPrices(str, result, len);

	for (int i=0; i<len; i++)
		cout << "symbol: " << result[i].symbol << ", last_price: " << YELLOW(result[i].price) << endl;
}

void Exchange::ShowBalances() {
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


