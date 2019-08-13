#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unistd.h>

#include <json/json.h>

#include "CurlAPI.h"
#include "Menu.h"
#include "Binance.h"
#include "Ramzinex.h"
#include "Coinex.h"
#include "HitBTC.h"
#include "Kucoin.h"
#include "BotMethod.h"
#include "Log.h"

// Some code to make terminal to have colors
#define KGRN "\033[0;32;32m"
#define KCYN "\033[0;36m"
#define KRED "\033[0;32;31m"
#define KYEL "\033[1;33m"
#define KBLU "\033[0;32;34m"
#define KCYN_L "\033[1;36m"
#define KBRN "\033[0;33m"
#define RESET "\033[0m"


using namespace std;


void Init() {
	CURL *curl;
	
	CurlAPI::CurlInit();
	curl = CurlAPI::GetCurl();

	Binance::Init();
	Binance::SetCurl(curl);

	Ramzinex::Init();
	Ramzinex::SetCurl(curl);

	Coinex::Init();
	Coinex::SetCurl(curl);

	HitBTC::Init();
	HitBTC::SetCurl(curl);

	Kucoin::Init();
	Kucoin::SetCurl(curl);

	// cout << "Successfully perform initialization\n";
}

void Cleanup() {
	CurlAPI::CurlCleanup();	

	Binance::Cleanup();
	Ramzinex::Cleanup();
	Coinex::Cleanup();
	HitBTC::Cleanup();
	Kucoin::Cleanup();


	// cout << "Successfully perform cleaning up\n";
}

void Exit() {
	cout << "Goodbye\n";
	Cleanup();
	exit(0);
}


//---------------------------
// Main function
//--------------------------
int main() {
	Init();
	
	while ( Menu::MainMenu() >= 0 );

	Exit();

	return 0;
}

