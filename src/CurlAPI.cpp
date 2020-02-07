#include "CurlAPI.h"
#include "binacpp_utils.h"


using namespace std;

CURL* CurlAPI::curl = NULL;
int CurlAPI::progressCnt = 0;
time_t CurlAPI::timeStart, CurlAPI::timeEnd;


void CurlAPI::CurlInit() {
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
}

void CurlAPI::CurlCleanup() {
	curl_easy_cleanup(curl);
	curl_global_cleanup();
}

//-----------------
// Curl's callback
//-----------------

static int getBarWidth () {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    // printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);
    return w.ws_col;
}

void CurlAPI::_waiting(future<void> futureObj) {
	int barWidth; // = 50;
	
	while (futureObj.wait_for(chrono::milliseconds(1)) == future_status::timeout) {
		time(&CurlAPI::timeEnd);
		
	    barWidth = (getBarWidth() - 30) * 0.75;

		cout << "\rPlease waiting      ";
	    cout << "[";
	    for (int i = 0; i < barWidth; ++i) {
	        if (i < progressCnt) cout << "*";
	        else if (i == progressCnt) cout << "*";
	        else cout << " ";
	    }
	    cout << "]   " << CurlAPI::timeEnd - CurlAPI::timeStart << " s";

	    if (++progressCnt > barWidth)
	    	progressCnt = 0;
		cout.flush();
		this_thread::sleep_for(chrono::milliseconds(200));
	}
	cout << "\r" << setfill(' ') << setw(getBarWidth()) << "" << "\r";
}

//-----------------
// Curl's callback
//-----------------
size_t CurlAPI::curl_cb(void *content, size_t size, size_t nmemb, string *buffer) {	
	// cout << "<BinaCPP::curl_cb>\n";

	buffer->append((char*)content, size*nmemb);

	// cout << "<BinaCPP::curl_cb> done\n";
	return size*nmemb;
}

int CurlAPI::curl_cb_debug(CURL *handle, curl_infotype type, char *data, size_t size, void *userp) {
	const char *text;
	(void)handle; /* prevent compiler warning */
	(void)userp;

	// cout << "\ncurl_cb_debug:\n";

	switch (type) {
		case CURLINFO_TEXT:
			cout << "== Info: \n";
			break;
		case CURLINFO_HEADER_OUT:
			cout << "=> Send header:\n";
			break;
		case CURLINFO_DATA_OUT:
			cout << "=> Send data:\n";
			break;
		case CURLINFO_SSL_DATA_OUT:
			cout << "=> Send SSL data:\n";
			break;
		case CURLINFO_HEADER_IN:
			cout << "<= Recv header:\n";
			break;
		case CURLINFO_DATA_IN:
			cout << "<= Recv data:\n";
			break;
		case CURLINFO_SSL_DATA_IN:
			cout << "<= Recv SSL data:\n";
			break;
		default: /* in case a new one is introduced to shock us */
			return 0;
	}
	
	cout << data << endl;
	// cout << "curl_cb_debug: END\n";

	return 0;
}


//-----------------
// Curl method functions
//-----------------
void CurlAPI::CurlGetRequest(string &url, string &str_result) {
	vector <string> v;
	string action = "GET";
	string post_data = "";
	string temp;
	curl_api_with_header(url, str_result, v, post_data, action, temp);
}

// Binance
void CurlAPI::CurlGetRequestWithMBXKey(string &url, string &str_result, string &api_key) {
	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	string action = "GET";
	string post_data = "";
	string temp;
	curl_api_with_header(url, str_result, extra_http_header, post_data, action, temp);
}

void CurlAPI::CurlPostRequestWithMBXKey(string &url, string &str_result, string &api_key) {
	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	string action = "POST";
	string post_data = "";
	string temp;
	curl_api_with_header(url, str_result, extra_http_header, post_data, action, temp);
}

void CurlAPI::CurlDeleteRequestWithMBXKey(string &url, string &str_result, string &api_key) {
	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	string action = "DELETE";
	string post_data = "";
	string temp;
	curl_api_with_header(url, str_result, extra_http_header, post_data, action, temp);
}

// Coinex
void CurlAPI::CurlGetRequestWithAuthorization(string &url, string &str_result, string &authorization) {
	vector <string> extra_http_header;
	string header_chunk("authorization: ");
	header_chunk.append( authorization );
	extra_http_header.push_back(header_chunk);

	string action = "GET";
	string post_data = "";
	string temp;
	curl_api_with_header(url, str_result, extra_http_header, post_data, action, temp);
}

void CurlAPI::CurlPostRequestWithAuthorization(string &url, string &str_result, string &authorization, string &post_data) {
	vector <string> extra_http_header;
	string header_chunk;

	header_chunk = "authorization: ";
	header_chunk.append(authorization);
	extra_http_header.push_back(header_chunk);

	header_chunk = "Accept: application/json";
	extra_http_header.push_back(header_chunk);
	header_chunk = "Content-Type: application/json";
	extra_http_header.push_back(header_chunk);
	header_chunk = "charsets: utf-8";
	extra_http_header.push_back(header_chunk);

	string action = "POST";
	string temp;
	curl_api_with_header(url, str_result, extra_http_header, post_data, action, temp);
}

void CurlAPI::CurlDeleteRequestWithAuthorization(string &url, string &str_result, string &authorization) {
	vector <string> extra_http_header;
	string header_chunk("authorization: ");
	header_chunk.append( authorization );
	extra_http_header.push_back(header_chunk);

	string action = "DELETE";
	string post_data = "";
	string temp;
	curl_api_with_header(url, str_result, extra_http_header, post_data, action, temp);
}

// HitBTC
void CurlAPI::CurlGetRequestWithBasicAuthentication(string &url, string &str_result, string &api_key, string &secret_key) {
	vector <string> extra_http_header;
	string header_chunk;
	extra_http_header.push_back(header_chunk);

	string action = "GET";
	string post_data = "";
	string user_pwd = api_key + ":" + secret_key;
	curl_api_with_header(url, str_result, extra_http_header, post_data, action, user_pwd);
}

void CurlAPI::CurlPostRequestWithBasicAuthentication(string &url, string &str_result, string &api_key, string &secret_key, string &post_data) {
	vector <string> extra_http_header;
	string header_chunk;

	header_chunk = "Accept: application/json";
	extra_http_header.push_back(header_chunk);
	header_chunk = "Content-Type: application/json";
	extra_http_header.push_back(header_chunk);
	header_chunk = "charsets: utf-8";
	extra_http_header.push_back(header_chunk);

	string action = "POST";
	string user_pwd = api_key + ":" + secret_key;
	curl_api_with_header(url, str_result, extra_http_header, post_data, action, user_pwd);
}

void CurlAPI::CurlDeleteRequestWithBasicAuthentication(string &url, string &str_result, string &api_key, string &secret_key) {
	vector <string> extra_http_header;
	string header_chunk;
	extra_http_header.push_back(header_chunk);

	string action = "DELETE";
	string post_data = "";
	string user_pwd = api_key + ":" + secret_key;
	curl_api_with_header(url, str_result, extra_http_header, post_data, action, user_pwd);
}

// Kucoin
void CurlAPI::CurlGetRequestWithKCKey(string &url, string &str_result, StructKucoinHttpFields &fields) {
	vector <string> extra_http_header;
	string header_chunk;
	header_chunk = "KC-API-KEY: " + fields.apiKey;
	extra_http_header.push_back(header_chunk);
	header_chunk = "KC-API-SIGN: " + fields.signature;
	extra_http_header.push_back(header_chunk);
	header_chunk = "KC-API-TIMESTAMP: " + fields.timestamp;
	extra_http_header.push_back(header_chunk);
	header_chunk = "KC-API-PASSPHRASE: " + fields.passphrase;
	extra_http_header.push_back(header_chunk);

	string action = "GET";
	string post_data = "";
	string temp;
	CurlCleanup();
	CurlInit();
	curl_api_with_header(url, str_result, extra_http_header, post_data, action, temp);
}

void CurlAPI::CurlPostRequestWithKCKey(string &url, string &str_result, StructKucoinHttpFields &fields, string &post_data) {
	vector <string> extra_http_header;
	string header_chunk;
	header_chunk = "KC-API-KEY: " + fields.apiKey;
	extra_http_header.push_back(header_chunk);
	header_chunk = "KC-API-SIGN: " + fields.signature;
	extra_http_header.push_back(header_chunk);
	header_chunk = "KC-API-TIMESTAMP: " + fields.timestamp;
	extra_http_header.push_back(header_chunk);
	header_chunk = "KC-API-PASSPHRASE: " + fields.passphrase;
	extra_http_header.push_back(header_chunk);

	string action = "POST";
	string temp;
	// CurlCleanup();
	// CurlInit();
	curl_api_with_header(url, str_result, extra_http_header, post_data, action, temp);
}

void CurlAPI::CurlDeleteRequestWithKCKey(string &url, string &str_result, StructKucoinHttpFields &fields) {
	vector <string> extra_http_header;
	string header_chunk;
	header_chunk = "KC-API-KEY: " + fields.apiKey;
	extra_http_header.push_back(header_chunk);
	header_chunk = "KC-API-SIGN: " + fields.signature;
	extra_http_header.push_back(header_chunk);
	header_chunk = "KC-API-TIMESTAMP: " + fields.timestamp;
	extra_http_header.push_back(header_chunk);
	header_chunk = "KC-API-PASSPHRASE: " + fields.passphrase;
	extra_http_header.push_back(header_chunk);

	string action = "DELETE";
	string post_data = "";
	string temp;
	// CurlCleanup();
	// CurlInit();
	curl_api_with_header(url, str_result, extra_http_header, post_data, action, temp);
}



//--------------------
// Do the curl
//--------------------
void CurlAPI::curl_api_with_header(string &url, string &str_result, vector <string> &extra_http_header, string &post_data, string &action, string user_pwd) {
	// Create a promise object
	promise<void> exitSignal;
 
	//Fetch future object associated with promise
	future<void> futureObj = exitSignal.get_future();
 
	// Starting Thread & move the future object in lambda function by reference
	thread th(&_waiting, move(futureObj));

	// cout << "<CurlAPI::curl_api>\n";

	CURLcode res;

	if( CurlAPI::curl ) {
		curl_easy_setopt(CurlAPI::curl, CURLOPT_URL, url.c_str() );
		curl_easy_setopt(CurlAPI::curl, CURLOPT_WRITEFUNCTION, CurlAPI::curl_cb);
		curl_easy_setopt(CurlAPI::curl, CURLOPT_DEBUGFUNCTION, CurlAPI::curl_cb_debug);
		curl_easy_setopt(CurlAPI::curl, CURLOPT_WRITEDATA, &str_result );
		curl_easy_setopt(CurlAPI::curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(CurlAPI::curl, CURLOPT_ENCODING, "gzip");
		// curl_easy_setopt(CurlAPI::curl, CURLOPT_FORBID_REUSE, 1L);
		// curl_easy_setopt(curl, CURLOPT_SSL_SESSIONID_CACHE, 0L);
		// curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		// curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		if (user_pwd.size() > 0)
			curl_easy_setopt(CurlAPI::curl, CURLOPT_USERPWD, user_pwd.c_str());

		if ( extra_http_header.size() > 0 ) {
			struct curl_slist *chunk = NULL;
			for ( int i = 0 ; i < extra_http_header.size() ;i++ ) {
				// cout << extra_http_header[i] << endl;
				chunk = curl_slist_append(chunk, extra_http_header[i].c_str() );
			}
			curl_easy_setopt(CurlAPI::curl, CURLOPT_HTTPHEADER, chunk);
		}

		if ( post_data.size() > 0 || action == "POST" || action == "PUT" || action == "DELETE" ) {
			if ( action == "PUT" || action == "DELETE" )
				curl_easy_setopt(CurlAPI::curl, CURLOPT_CUSTOMREQUEST, action.c_str() );
			curl_easy_setopt(CurlAPI::curl, CURLOPT_POSTFIELDS, post_data.c_str() );
			// cout << post_data << endl;
 		}

		res = curl_easy_perform(CurlAPI::curl);

		/* Check for errors */ 
		if ( res != CURLE_OK )
			cout << "<CurlAPI::curl_api> curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;

		//Set the value in promise
		exitSignal.set_value();
	 
		//Wait for thread to join
		th.join();
	}

	// cout << "<CurlAPI::curl_api> done\n";
}


//--------------------
// Get IP address
//--------------------
void CurlAPI::ShowExternalIpAddress() {
	// cout << "ShowExternalIpAddress\n";
	string str_result;
	string url;

	// Get ip address
	url = GET_IP_ADDRESS_HOST;
	// cout << url << endl;
	CurlAPI::CurlGetRequest(url, str_result);
	// cout << "inside ShowExternalIpAddress, str_result: " << str_result << endl;

	string IP;
	if (str_result.size() > 0 ) {
		IP = str_result;
		// cout << "IP: " << IP << endl;
	} else {
		cout << "Cannot get ip address from " << url << endl;
		return;
	}


	// Get location from ip address
	url = GET_LOCATION_FRON_IP_ADDRESS + IP;
	cout << url << endl;
	str_result.clear();
	CurlAPI::CurlGetRequest(url, str_result);

	Json::Value json_result;
	Utility::ParseStringToJson("ShowExternalIpAddress", str_result, json_result);
	// cout << "Inside ShowExternalIpAddress, str_result: " << str_result << endl;
	// cout << "Inside ShowExternalIpAddress, json_result: \n" << json_result << endl;

	if (str_result.size() > 0 ) {
		// cout << str_result << endl;
		cout << json_result << endl;
	} else {
		cout << "Cannot get ip address from " << url << endl;
		return;
	}
}
