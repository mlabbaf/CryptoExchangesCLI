#include "Ramzinex.h"
#include "CurlAPI.h"


using namespace std;

CURL* Ramzinex::curl = NULL;
Json::Value Ramzinex::json_result;
Json::FastWriter Ramzinex::fastWriter;


void Ramzinex::Init() {
	// cout << "Successfully perform Ramzinex initialization\n";
}

void Ramzinex::Cleanup() {
	// cout << "Successfully perform Ramzinex cleaning up\n";
}

void Ramzinex::_GetAllPrices() {
	// cout << "GetAllPrices\n";
	string url(RAMZINEX_HOST);  
	url += "/prices";

	string str_result;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside GetAllPrices, str_result: " << str_result << endl;

	if ( str_result.size() > 0 ) {		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
		} catch ( exception &e ) {
		 	cout << "<Ramzinex:GetAllPrices> Error ! " << e.what() << endl; 
		}   
		// cout << "<Ramzinex:GetAllPrices>Done." << endl ;
	} else
		cout << "<Ramzinex:GetAllPrices>Failed to get anything." << endl;
}

// This function not worked complete in WatchList situation
Json::Value Ramzinex::GetPrices(string &str) {
	Ramzinex::_GetAllPrices();

	transform(str.begin(), str.end(), str.begin(), ::tolower);

	if (str == "all")
		return json_result;
	else if (str == "watchlist") {
		int counter = 1;
		string line;
  		ifstream myfile ("config/WatchlistRamzinex.txt");
  		if (myfile.is_open()) {
    		while ( getline (myfile,line) ) {
    			transform(line.begin(), line.end(), line.begin(), ::tolower);
				// cout << counter++ << ")  " << line << endl << json_result[line] << endl;
				cout << line << ":      \t" << fastWriter.write(json_result[line]);
    		}
    		myfile.close();
    	}
  		else 
  			cout << "Unable to open file config/WatchlistRamzinex.txt\n"; 
  		// cout << "GetPrices function not worked complete in WatchList situation\n";
  		cout << endl;
  		Json::Value temp;
  		return temp;  // not worked correctly in this situation
  	}
	else
		return json_result[str];
}

void Ramzinex::ShowPrices(string str) {
	Ramzinex::GetPrices(str);

	transform(str.begin(), str.end(), str.begin(), ::tolower);

	if (str == "all")
		cout << json_result << endl; 
		// cout << fastWriter.write(json_result) << endl;
	else if (str == "watchlist") {}		// Handled in GetPrices
	else
		// cout << json_result[str] << endl;
		cout << str << endl << fastWriter.write(json_result[str]) << endl;
}
