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
#include "Nobitex.h"
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

static Exchange *binance;
static Exchange *coinex;
static Exchange *hitbtc;
static Exchange *kucoin;
static Exchange *nobitex;


void Init() {
	CURL *curl;
	
	CurlAPI::CurlInit();
	curl = CurlAPI::GetCurl();

	binance = Binance::getInstance();
	coinex = Coinex::getInstance();
	hitbtc = HitBTC::getInstance();
	kucoin = Kucoin::getInstance();
	nobitex = Nobitex::getInstance();

	binance->setExchangeName("Binance");
	binance->Init();
	binance->SetCurl(curl);

	coinex->setExchangeName("Coinex");
	coinex->Init();
	coinex->SetCurl(curl);

	hitbtc->setExchangeName("HitBTC");
	hitbtc->Init();
	hitbtc->SetCurl(curl);

	kucoin->setExchangeName("Kucoin");
	kucoin->Init();
	kucoin->SetCurl(curl);

	nobitex->setExchangeName("Nobitex");
	nobitex->Init();
	nobitex->SetCurl(curl);
	
	Ramzinex::Init();
	Ramzinex::SetCurl(curl);
	
	Menu::Init();
	BotMethod::Init();

	// cout << "Successfully perform initialization\n";
}

void Cleanup() {
	CurlAPI::CurlCleanup();	

	binance->Cleanup();
	Ramzinex::Cleanup();
	coinex->Cleanup();
	hitbtc->Cleanup();
	kucoin->Cleanup();
	nobitex->Cleanup();

	Menu::Cleanup();
	BotMethod::Cleanup();

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

