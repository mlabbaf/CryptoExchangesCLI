#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <exception>
#include <fstream>
#include <algorithm>
#include <sys/time.h>
#include <iomanip>

#include <json/json.h>

#include "Log.h"


using namespace std;


class Utility
{
public:
	static double JsonToDouble(Json::Value json_val) {
		return atof(json_val.asString().c_str());
	}

	static long JsonToLong(Json::Value json_val) {
		return atol(json_val.asString().c_str());
	}

	static void ParseStringToJson(string log, string& str, Json::Value &result) {
		result.clear();

		// cout << "_parseStringToJson: " << YELLOW(str) << endl;

		if (str.size() > 0) {
			try {
				Json::Reader reader;
				result.clear();	
				reader.parse( str , result);
		    		
			} catch ( exception &e ) {
			 	cout << log << " Error ! " << e.what() << endl; 
				cout << "str: " << str << endl;
			}
			// cout << log << " Done.\n";
		} else {
			cout << log << " Failed to get anything.\n";
			cout << "str: " << str << endl;
		}
	}

	static void RoundAmountBasedOnPair(string pair, double &amount) {

		// cout << "pair " << pair;
		// cout << " before " << amount;

		if (pair == "BTCUSDT" || pair == "BTCUSD")
			amount -= fmod(amount, 0.0001);
		else if (pair == "ETHUSDT" || pair == "ETHUSD")
			amount -= fmod(amount, 0.001);
		else if (pair == "NEOUSDT" || pair == "NEOUSD")
			amount -= fmod(amount, 0.01);
		else if (pair == "ONTUSDT" || pair == "ONTUSD")
			amount -= fmod(amount, 0.01);
		else if (pair == "ADAUSDT" || pair == "ADAUSD")
			amount -= fmod(amount, 1);
		else if (pair == "TRXUSDT" || pair == "TRXUSD")
			amount -= fmod(amount, 1);
		else if (pair == "XLMUSDT" || pair == "XLMUSD")
			amount -= fmod(amount, 0.1);
		else if (pair == "XRPUSDT")
			amount -= fmod(amount, 0.01);
		else if (pair == "IOTAUSDT" || pair == "IOTAUSD")
			amount -= fmod(amount, 0.1);
		else if (pair == "BTTUSDT" || pair == "BTTUSD")
			amount -= fmod(amount, 1);
		else if (pair == "BNBUSDT" || pair == "BNBUSD")
			amount -= fmod(amount, 0.01);
		else if (pair == "LTCUSDT" || pair == "LTCUSD")
			amount -= fmod(amount, 0.01);
		else {
			cout << "Invalid pair " << RED(pair) << endl;
			// amount = 0;
		}

		// cout << " after " << amount << endl;
	}

	static void RoundPriceBasedOnPair(string pair, double &amount) {

		// cout << "pair " << pair;
		// cout << " before " << amount << endl;

		if (pair == "BTCUSDT" || pair == "BTCUSD")
			amount -= fmod(amount, 1);
		else if (pair == "ETHUSDT" || pair == "ETHUSD")
			amount -= fmod(amount, 0.01);
		else if (pair == "NEOUSDT" || pair == "NEOUSD")
			amount -= fmod(amount, 0.001);
		else if (pair == "ONTUSDT" || pair == "ONTUSD")
			amount -= fmod(amount, 0.0001);
		else if (pair == "ADAUSDT" || pair == "ADAUSD")
			amount -= fmod(amount, 0.001);
		else if (pair == "TRXUSDT" || pair == "TRXUSD")
			amount -= fmod(amount, 0.0001);
		else if (pair == "XLMUSDT" || pair == "XLMUSD")
			amount -= fmod(amount, 0.0001);
		else if (pair == "XRPUSDT")
			amount -= fmod(amount, 0.001);
		else if (pair == "IOTAUSDT" || pair == "IOTAUSD")
			amount -= fmod(amount, 0.001);
		else if (pair == "BTTUSDT" || pair == "BTTUSD")
			amount -= fmod(amount, 0.000001);
		else if (pair == "BNBUSDT" || pair == "BNBUSD")
			amount -= fmod(amount, 0.001);
		else if (pair == "LTCUSDT" || pair == "LTCUSD")
			amount -= fmod(amount, 0.001);
		else {
			cout << "Invalid pair " << RED(pair) << endl;
			// amount = 0;
		}

		// cout << " after " << amount << endl;
	}

	static bool AreYouSure(string str) {
		char sure;
		cout << str << endl;
		cout << "If you sure to continue, press y:\n";
		cin >> sure;
		if (sure == 'y' || sure == 'Y')
			return true;
		else 
			return false;
	}

	static string tostring(const double &n) {
    	string s;
    	if (n < 0.01*0.01) {
	    	ostringstream oss;
	    	oss << fixed << setprecision(20);
	    	oss << n;
	    	s =  oss.str();
	    	int dotpos = s.find_first_of('.');
	    	if(dotpos!=string::npos){
	        	int ipos = s.size()-1;
	        	while(s[ipos]=='0' && ipos>dotpos){
	            	--ipos;
	        	}
	        	s.erase ( ipos + 1, string::npos );
	    	}
    		
    	}
    	else 
    		s = to_string(n);
    	return s;
	}
	
};

#endif