#include "Menu.h"
#include "Log.h"
#include "Binance.h"
#include "Ramzinex.h"
#include "Coinex.h"
#include "HitBTC.h"
#include "Kucoin.h"
#include "BotMethod.h"
#include "CurlAPI.h"

using namespace std;

static Binance *binance;
static Coinex *coinex;
static HitBTC *hitbtc;
static Kucoin *kucoin;


static void Exit() {
	cout << "Goodbye\n";
	exit(0);
}

static void test() {
	cout << KGRN << "test" << RESET << endl;
}

enum _SubMenuListMode {
	WITH_ALL_AND_WATCHLIST = 0,
	WITH_ALL,
	WITH_WATCHLIST,
	WITHOUT_ALL_AND_WATCHLIST
};


//-----------------
// Utilities
//-----------------
static void _NotSupported() {
	cout << RED("This function is not supported\n");
}

static string _ExchangeSubMenuList(int mode) {
	int choice;

	map <int, string> MenuOption;
	if (mode & ALL)
		MenuOption.insert(pair <int, string> (1, "All"));
	if (mode & BINANCE)
		MenuOption.insert(pair <int, string> (2, "Binance"));
	if (mode & RAMZINEX)
		MenuOption.insert(pair <int, string> (3, "Ramzinex"));
	if (mode & COINEX)
		MenuOption.insert(pair <int, string> (4, "Coinex"));
	if (mode & HITBTC)
		MenuOption.insert(pair <int, string> (5, "HitBTC"));
	if (mode & KUCOIN)
		MenuOption.insert(pair <int, string> (6, "Kucoin"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static int _DurationPerDay() {
	int choice;

	map <int, DayDurationMapValues> MenuOption;
	MenuOption.insert(pair <int, DayDurationMapValues> (0, DayDurationMapValues("All", 0)));
	MenuOption.insert(pair <int, DayDurationMapValues> (1, DayDurationMapValues("1-Day", 1)));
	MenuOption.insert(pair <int, DayDurationMapValues> (3, DayDurationMapValues("3-Days", 3)));
	MenuOption.insert(pair <int, DayDurationMapValues> (7, DayDurationMapValues("1-Week", 7)));
	MenuOption.insert(pair <int, DayDurationMapValues> (14, DayDurationMapValues("2-Weeks", 14)));
	MenuOption.insert(pair <int, DayDurationMapValues> (21, DayDurationMapValues("3-Weeks", 21)));
	MenuOption.insert(pair <int, DayDurationMapValues> (24, DayDurationMapValues("24-Days (MAX)", 24)));

	cout << "Select from following menu:\n";

	map<int, DayDurationMapValues>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second.description << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;

	int rc = -1;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second.Day;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static string _OrderSideList() {
	int choice;

	map <int, string> MenuOption;
	MenuOption.insert(pair <int, string> (1, "SELL"));
	MenuOption.insert(pair <int, string> (2, "BUY"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static double _GetDoubleValue(string DisplayString) {
	cout << DisplayString << endl;
	
	double rc = 0;
	cin >> rc;

	if (rc <= 0)
		cout << RED("Invalid number\n\n\n");
	return rc;	
}

static long _GetLongValue(string DisplayString) {
	cout << DisplayString << endl;
	
	long rc = 0;
	cin >> rc;

	if (rc <= 0)
		cout << RED("Invalid number\n\n\n");
	return rc;	
}

static string _GetStringValue(string DisplayString) {
	cout << DisplayString << endl;
	
	string rc;
	cin >> rc;

	if (rc.size() == 0)
		cout << RED("Invalid number\n\n\n");
	return rc;		
}

// Binance
static string _BinancePairSubMenuList(_SubMenuListMode mode) {
	int choice;

	map <int, string> MenuOption;
	if (mode == WITH_ALL || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (1, "All"));
	if (mode == WITH_WATCHLIST || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (2, "WatchList"));
	MenuOption.insert(pair <int, string> (3, "BTCUSDT"));
	MenuOption.insert(pair <int, string> (4, "ETHUSDT"));
	MenuOption.insert(pair <int, string> (5, "NEOUSDT"));
	MenuOption.insert(pair <int, string> (6, "ONTUSDT"));
	MenuOption.insert(pair <int, string> (7, "ADAUSDT"));
	MenuOption.insert(pair <int, string> (8, "TRXUSDT"));
	MenuOption.insert(pair <int, string> (9, "XLMUSDT"));
	MenuOption.insert(pair <int, string> (10, "XRPUSDT"));
	MenuOption.insert(pair <int, string> (11, "IOTAUSDT"));
	MenuOption.insert(pair <int, string> (12, "BTTUSDT"));
	MenuOption.insert(pair <int, string> (13, "BNBUSDT"));
	MenuOption.insert(pair <int, string> (14, "LTCUSDT"));
	MenuOption.insert(pair <int, string> (15, "DENTUSDT"));
	MenuOption.insert(pair <int, string> (16, "LINKUSDT"));
	MenuOption.insert(pair <int, string> (17, "EOSUSDT"));
	MenuOption.insert(pair <int, string> (18, "SHIBUSDT"));
	MenuOption.insert(pair <int, string> (19, "BALUSDT"));
	MenuOption.insert(pair <int, string> (20, "DOGEUSDT"));
	MenuOption.insert(pair <int, string> (21, "ALPHAUSDT"));
	MenuOption.insert(pair <int, string> (22, "MATICUSDT"));
	// MenuOption.insert(pair <int, string> (101, "DENTBTC"));
	MenuOption.insert(pair <int, string> (102, "COSBTC"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

string Menu::binancePairSubMenuList() {
	return _BinancePairSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
}

static string _BinanceSingleSubMenuList(_SubMenuListMode mode) {
	int choice;

	map <int, string> MenuOption;
	if (mode == WITH_ALL || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (1, "All"));
	if (mode == WITH_WATCHLIST || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (2, "WatchList"));
	MenuOption.insert(pair <int, string> (3, "BTC"));
	MenuOption.insert(pair <int, string> (4, "ETH"));
	MenuOption.insert(pair <int, string> (5, "NEO"));
	MenuOption.insert(pair <int, string> (6, "ONT"));
	MenuOption.insert(pair <int, string> (7, "ADA"));
	MenuOption.insert(pair <int, string> (8, "TRX"));
	MenuOption.insert(pair <int, string> (9, "XLM"));
	MenuOption.insert(pair <int, string> (10, "XRP"));
	MenuOption.insert(pair <int, string> (11, "IOTA"));
	MenuOption.insert(pair <int, string> (12, "BTT"));
	MenuOption.insert(pair <int, string> (13, "BNB"));
	MenuOption.insert(pair <int, string> (14, "LTC"));
	MenuOption.insert(pair <int, string> (15, "DENT"));
	MenuOption.insert(pair <int, string> (16, "LINK"));
	MenuOption.insert(pair <int, string> (21, "GAS"));
	MenuOption.insert(pair <int, string> (101, "USDT"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static string _BinanceNetworkSubMenuList() {
	int choice;

	map <int, string> MenuOption;
	MenuOption.insert(pair <int, string> (101, "isDefault"));
	MenuOption.insert(pair <int, string> (102, "BTC"));
	// MenuOption.insert(pair <int, string> (103, "SEGWITBTC"));
	MenuOption.insert(pair <int, string> (104, "OMNI"));
	MenuOption.insert(pair <int, string> (105, "ETH"));
	// MenuOption.insert(pair <int, string> (106, "NEO"));
	// MenuOption.insert(pair <int, string> (107, "ONT"));
	// MenuOption.insert(pair <int, string> (108, "ADA"));
	MenuOption.insert(pair <int, string> (109, "TRX"));
	// MenuOption.insert(pair <int, string> (110, "XLM"));
	// MenuOption.insert(pair <int, string> (111, "XRP"));
	// MenuOption.insert(pair <int, string> (112, "IOTA"));
	MenuOption.insert(pair <int, string> (113, "BNB"));
	MenuOption.insert(pair <int, string> (114, "BSC"));
	// MenuOption.insert(pair <int, string> (115, "LTC"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static string _BinanceOrderTypeList() {
	int choice;

	map <int, string> MenuOption;
	MenuOption.insert(pair <int, string> (1, "LIMIT"));
	MenuOption.insert(pair <int, string> (2, "MARKET"));
	// MenuOption.insert(pair <int, string> (3, "STOP_LOSS"));
	MenuOption.insert(pair <int, string> (4, "STOP_LOSS_LIMIT"));
	// MenuOption.insert(pair <int, string> (5, "TAKE_PROFIT"));
	// MenuOption.insert(pair <int, string> (6, "TAKE_PROFIT_LIMIT"));
	// MenuOption.insert(pair <int, string> (7, "LIMIT_MAKER"));
	MenuOption.insert(pair <int, string> (8, "OCO"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

// Coinex
static string _CoinexPairSubMenuList(_SubMenuListMode mode) {
	int choice;

	map <int, string> MenuOption;
	if (mode == WITH_ALL || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (1, "All"));
	if (mode == WITH_WATCHLIST || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (2, "WatchList"));
	MenuOption.insert(pair <int, string> (3, "BTCUSDT"));
	MenuOption.insert(pair <int, string> (4, "ETHUSDT"));
	MenuOption.insert(pair <int, string> (5, "NEOUSDT"));
	MenuOption.insert(pair <int, string> (6, "ONTUSDT"));
	MenuOption.insert(pair <int, string> (7, "ADAUSDT"));
	MenuOption.insert(pair <int, string> (8, "TRXUSDT"));
	MenuOption.insert(pair <int, string> (9, "XLMUSDT"));
	MenuOption.insert(pair <int, string> (10, "XRPUSDT"));
	MenuOption.insert(pair <int, string> (13, "CETUSDT"));
	MenuOption.insert(pair <int, string> (14, "LTCUSDT"));
	MenuOption.insert(pair <int, string> (17, "EOSUSDT"));
	MenuOption.insert(pair <int, string> (20, "DOGEUSDT"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static string _CoinexSingleSubMenuList(_SubMenuListMode mode) {
	int choice;

	map <int, string> MenuOption;
	if (mode == WITH_ALL || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (1, "All"));
	if (mode == WITH_WATCHLIST || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (2, "WatchList"));
	MenuOption.insert(pair <int, string> (3, "BTC"));
	MenuOption.insert(pair <int, string> (4, "ETH"));
	MenuOption.insert(pair <int, string> (5, "NEO"));
	MenuOption.insert(pair <int, string> (6, "ONT"));
	MenuOption.insert(pair <int, string> (7, "ADA"));
	MenuOption.insert(pair <int, string> (8, "TRX"));
	MenuOption.insert(pair <int, string> (9, "XLM"));
	MenuOption.insert(pair <int, string> (10, "XRP"));
	MenuOption.insert(pair <int, string> (13, "CET"));
	MenuOption.insert(pair <int, string> (14, "LTC"));
	MenuOption.insert(pair <int, string> (101, "USDT"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static string _CoinexOrderTypeList() {
	int choice;

	map <int, string> MenuOption;
	MenuOption.insert(pair <int, string> (1, "LIMIT"));
	MenuOption.insert(pair <int, string> (2, "MARKET"));
	// MenuOption.insert(pair <int, string> (3, "STOP_LOSS"));
	// MenuOption.insert(pair <int, string> (4, "STOP_LOSS_LIMIT"));
	// MenuOption.insert(pair <int, string> (5, "TAKE_PROFIT"));
	// MenuOption.insert(pair <int, string> (6, "TAKE_PROFIT_LIMIT"));
	// MenuOption.insert(pair <int, string> (7, "LIMIT_MAKER"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

// HitBTC
static string _HitBTCPairSubMenuList(_SubMenuListMode mode) {
	int choice;

	map <int, string> MenuOption;
	if (mode == WITH_ALL || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (1, "All"));
	if (mode == WITH_WATCHLIST || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (2, "WatchList"));
	MenuOption.insert(pair <int, string> (3, "BTCUSD"));
	MenuOption.insert(pair <int, string> (4, "ETHUSD"));
	MenuOption.insert(pair <int, string> (5, "NEOUSD"));
	MenuOption.insert(pair <int, string> (6, "ONTUSD"));
	MenuOption.insert(pair <int, string> (7, "ADAUSD"));
	MenuOption.insert(pair <int, string> (8, "TRXUSD"));
	MenuOption.insert(pair <int, string> (9, "XLMUSD"));
	MenuOption.insert(pair <int, string> (10, "XRPUSDT"));
	MenuOption.insert(pair <int, string> (11, "IOTAUSD"));
	MenuOption.insert(pair <int, string> (12, "BTTUSD"));
	MenuOption.insert(pair <int, string> (13, "BNBUSD"));
	MenuOption.insert(pair <int, string> (14, "LTCUSD"));
	MenuOption.insert(pair <int, string> (15, "DENTUSD"));
	MenuOption.insert(pair <int, string> (16, "LINKUSD"));
	MenuOption.insert(pair <int, string> (17, "EOSUSD"));
	MenuOption.insert(pair <int, string> (104, "ETHBTC"));
	MenuOption.insert(pair <int, string> (110, "XRPBTC"));
	MenuOption.insert(pair <int, string> (115, "DENTBTC"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static string _HitBTCSingleSubMenuList(_SubMenuListMode mode) {
	int choice;

	map <int, string> MenuOption;
	if (mode == WITH_ALL || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (1, "All"));
	if (mode == WITH_WATCHLIST || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (2, "WatchList"));
	MenuOption.insert(pair <int, string> (3, "BTC"));
	MenuOption.insert(pair <int, string> (4, "ETH"));
	MenuOption.insert(pair <int, string> (5, "NEO"));
	MenuOption.insert(pair <int, string> (6, "ONT"));
	MenuOption.insert(pair <int, string> (7, "ADA"));
	MenuOption.insert(pair <int, string> (8, "TRX"));
	MenuOption.insert(pair <int, string> (9, "XLM"));
	MenuOption.insert(pair <int, string> (10, "XRP"));
	MenuOption.insert(pair <int, string> (11, "IOTA"));
	MenuOption.insert(pair <int, string> (12, "BTT"));
	MenuOption.insert(pair <int, string> (13, "BNB"));
	MenuOption.insert(pair <int, string> (14, "LTC"));
	MenuOption.insert(pair <int, string> (15, "DENT"));
	MenuOption.insert(pair <int, string> (16, "LINK"));
	MenuOption.insert(pair <int, string> (21, "GAS"));
	MenuOption.insert(pair <int, string> (101, "USD"));
	MenuOption.insert(pair <int, string> (102, "USDT20"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static string _HitBTCOrderTypeList() {
	int choice;

	map <int, string> MenuOption;
	MenuOption.insert(pair <int, string> (1, "limit"));
	MenuOption.insert(pair <int, string> (2, "market"));
	// MenuOption.insert(pair <int, string> (3, "STOP_LOSS"));
	MenuOption.insert(pair <int, string> (4, "stopLimit"));
	// MenuOption.insert(pair <int, string> (5, "TAKE_PROFIT"));
	// MenuOption.insert(pair <int, string> (6, "TAKE_PROFIT_LIMIT"));
	// MenuOption.insert(pair <int, string> (7, "LIMIT_MAKER"));
	// MenuOption.insert(pair <int, string> (8, "stopMarket"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static string _HitBTCTransferType() {
	int choice;

	map <int, string> MenuOption;
	MenuOption.insert(pair <int, string> (1, "bankToExchange"));
	MenuOption.insert(pair <int, string> (2, "exchangeToBank"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

// Kucoin
static string _KucoinPairSubMenuList(_SubMenuListMode mode) {
	int choice;

	map <int, string> MenuOption;
	if (mode == WITH_ALL || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (1, "All"));
	if (mode == WITH_WATCHLIST || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (2, "WatchList"));
	MenuOption.insert(pair <int, string> (3, "BTC-USDT"));
	MenuOption.insert(pair <int, string> (4, "ETH-USDT"));
	MenuOption.insert(pair <int, string> (5, "NEO-USDT"));
	MenuOption.insert(pair <int, string> (6, "ONT-USDT"));
	MenuOption.insert(pair <int, string> (7, "ADA-USDT"));
	MenuOption.insert(pair <int, string> (8, "TRX-USDT"));
	MenuOption.insert(pair <int, string> (9, "XLM-USDT"));
	MenuOption.insert(pair <int, string> (10, "XRP-USDT"));
	MenuOption.insert(pair <int, string> (11, "IOT-USDT"));
	// MenuOption.insert(pair <int, string> (12, "BTT-USDT"));
	MenuOption.insert(pair <int, string> (13, "KCS-USDT"));
	MenuOption.insert(pair <int, string> (14, "LTC-USDT"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static string _KucoinSingleSubMenuList(_SubMenuListMode mode) {
	int choice;

	map <int, string> MenuOption;
	if (mode == WITH_ALL || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (1, "All"));
	if (mode == WITH_WATCHLIST || mode == WITH_ALL_AND_WATCHLIST)
		MenuOption.insert(pair <int, string> (2, "WatchList"));
	MenuOption.insert(pair <int, string> (3, "BTC"));
	MenuOption.insert(pair <int, string> (4, "ETH"));
	MenuOption.insert(pair <int, string> (5, "NEO"));
	MenuOption.insert(pair <int, string> (6, "ONT"));
	MenuOption.insert(pair <int, string> (7, "ADA"));
	MenuOption.insert(pair <int, string> (8, "TRX"));
	MenuOption.insert(pair <int, string> (9, "XLM"));
	MenuOption.insert(pair <int, string> (10, "XRP"));
	MenuOption.insert(pair <int, string> (11, "IOTA"));
	// MenuOption.insert(pair <int, string> (12, "BTT"));
	MenuOption.insert(pair <int, string> (13, "KCS"));
	MenuOption.insert(pair <int, string> (14, "LTC"));
	MenuOption.insert(pair <int, string> (21, "GAS"));
	MenuOption.insert(pair <int, string> (101, "USDT"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static string _KucoinOrderTypeList() {
	int choice;

	map <int, string> MenuOption;
	MenuOption.insert(pair <int, string> (1, "limit"));
	MenuOption.insert(pair <int, string> (2, "market"));
	// MenuOption.insert(pair <int, string> (3, "STOP_LOSS"));
	// MenuOption.insert(pair <int, string> (4, "stopLimit"));
	// MenuOption.insert(pair <int, string> (5, "TAKE_PROFIT"));
	// MenuOption.insert(pair <int, string> (6, "TAKE_PROFIT_LIMIT"));
	// MenuOption.insert(pair <int, string> (7, "LIMIT_MAKER"));
	// MenuOption.insert(pair <int, string> (8, "stopMarket"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = MenuOption.begin(); it != MenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;


	string rc;
	it = MenuOption.find(choice);
	if (it != MenuOption.end())
		rc = it->second;
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
	
	return rc;
}

static string _KucoinTransferType() {
	return _HitBTCTransferType();
}

//-----------------
// All exchanges
//-----------------
static void ShowAllPrice(int mode) {
	// if (mode & BINANCE)
	// 	binance->ShowPrices(string("WatchList"));
	// if (mode & RAMZINEX)
	// 	Ramzinex::ShowPrices(string("WatchList"));
	// if (mode & COINEX)
	// 	coinex->ShowPrices(string("WatchList"));
	// if (mode & HITBTC)
	// 	hitbtc->ShowPrices(string("WatchList"));
	// if (mode & KUCOIN)
	// 	kucoin->ShowPrices(string("WatchList"));
	BotMethod::ShowAllExchangesPrice(mode);
}

static void ShowAllOpenOrders(int mode) {
	if (mode & BINANCE)
		binance->ShowOpenOrders(string("All"));
	if (mode & RAMZINEX)
		_NotSupported();
	if (mode & COINEX)
		coinex->ShowOpenOrders(string("WatchList"));
	if (mode & HITBTC)
		hitbtc->ShowOpenOrders(string("All"));
	if (mode & KUCOIN)
		kucoin->ShowOpenOrders(string("All"));
}

static void ShowAllMyTrades (int mode) {
	int pastDay = _DurationPerDay();
	// cout << YELLOW("Warning: ") << "Binance, HitBTC and Coinex\n";
	
	if (pastDay >= 0) {
		if (mode & BINANCE)
			binance->ShowMyTrades(string("WatchList"), pastDay);
		if (mode & RAMZINEX)
			_NotSupported();
		if (mode & COINEX)
			coinex->ShowMyTrades(string("WatchList"), pastDay);
		if (mode & HITBTC)
			hitbtc->ShowMyTrades(string("WatchList"), pastDay);
		if (mode & KUCOIN)
			kucoin->ShowMyTrades(string("All"), pastDay);
	}
}

static void ShowAllDepositHistory (int mode) {
	if (mode & BINANCE)
		binance->ShowDepositHistory(string("All"));
	if (mode & RAMZINEX)
		_NotSupported();
	if (mode & COINEX)
		coinex->ShowDepositHistory(string("WatchList"));
	if (mode & HITBTC)
		_NotSupported();
	if (mode & KUCOIN)
		kucoin->ShowDepositHistory(string("All"));
}

static void ShowAllWithdrawHistory (int mode) {
	if (mode & BINANCE)
		binance->ShowWithdrawHistory(string("All"));
	if (mode & RAMZINEX)
		_NotSupported();
	if (mode & COINEX)
		coinex->ShowWithdrawHistory(string("WatchList"));
	if (mode & HITBTC)
		_NotSupported();
	if (mode & KUCOIN)
		kucoin->ShowDepositHistory(string("All"));
}

//-----------------
// Binance
//-----------------
static void ShowBinancePrice() {
	// cout << "Inside ShowBinancePrice\n";

	string SubMenuSelection = _BinancePairSubMenuList(WITH_ALL_AND_WATCHLIST);
	// cout << "ShowBinancePrice: " << SubMenuSelection << endl;
	if (SubMenuSelection.size() != 0)
		binance->ShowPrices(SubMenuSelection);
}

static void ShowBinanceOpenOrders() {
	// cout << "Inside ShowBinanceOpenOrders\n";

	string SubMenuSelection = _BinancePairSubMenuList(WITH_ALL);
	if (SubMenuSelection.size() != 0)
		binance->ShowOpenOrders(SubMenuSelection);
}

static void ShowBinanceAllOrders() {
	// cout << "Inside ShowBinanceAllOrders\n";

	string SubMenuSelection = _BinancePairSubMenuList(WITH_WATCHLIST);
	if (SubMenuSelection.size() != 0) {
		int pastDay = _DurationPerDay();
		if (pastDay >= 0)
			binance->ShowAllOrders(SubMenuSelection, pastDay);
	}
}

static void ShowBinanceMyTrades () {
	// cout << "Inside ShowBinanceMyTrades\n";

	string SubMenuSelection = _BinancePairSubMenuList(WITH_WATCHLIST);
	if (SubMenuSelection.size() != 0) {
		int pastDay = _DurationPerDay();
		if (pastDay >= 0)
			binance->ShowMyTrades(SubMenuSelection, pastDay);	
	}
}

static void ShowBinanceTradesPerformance () {
	// cout << "Inside ShowBinanceTradesPerformance\n";

	string SubMenuSelection = _BinancePairSubMenuList(WITH_WATCHLIST);
	if (SubMenuSelection.size() != 0) {
		int pastDay = _DurationPerDay();
		if (pastDay >= 0)
			binance->ShowTradesPerformance(SubMenuSelection, pastDay);	
	}
}

static void ShowBinanceDepositAddress () {
	// cout << "Inside ShowBinanceDepositAddress\n";

	string SubMenuSelection = _BinanceSingleSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (SubMenuSelection.size() != 0) {
		string SubMenuSelectionNetwork = _BinanceNetworkSubMenuList();
		if (SubMenuSelectionNetwork.size() != 0)
			binance->ShowDepositAddress(SubMenuSelection, SubMenuSelectionNetwork);
	}
}

static void ShowBinanceDepositHistory () {
	// cout << "Inside ShowBinanceDepositHistory\n";

	string SubMenuSelection = _BinanceSingleSubMenuList(WITH_ALL);
	if (SubMenuSelection.size() != 0)
		binance->ShowDepositHistory(SubMenuSelection);	
}

static void ShowBinanceWithdrawHistory () {
	// cout << "Inside ShowBinanceWithdrawHistory\n";

	string SubMenuSelection = _BinanceSingleSubMenuList(WITH_ALL);
	if (SubMenuSelection.size() != 0)
		binance->ShowWithdrawHistory(SubMenuSelection);	
}

static void SendOrderToBinance() {
	string symbol = _BinancePairSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (symbol.size() != 0) {
		string side = _OrderSideList();
		if (side.size() != 0) {
			string type = _BinanceOrderTypeList();
			if (type.size() != 0) {
				double quantity = _GetDoubleValue("Enter the quantity:");
				if (quantity > 0) {
					if (type != "MARKET") {
						double price = _GetDoubleValue("Enter the price:");
						if (price > 0) {
							if (type != "OCO")
								binance->SendOrder(symbol, side, type, quantity, price, 0, 0);
							else {
								double stopPrice = _GetDoubleValue("Enter the stopPrice:");
								if (stopPrice > 0) {
									double stopLimitPrice = _GetDoubleValue("Enter the stopLimitPrice:");
									if (stopLimitPrice > 0)
										binance->SendOrder(symbol, side, type, quantity, price, stopPrice, stopLimitPrice);
								}
							}
						}
					}
					else 
						binance->SendOrder(symbol, side, type, quantity, 0, 0, 0);
				}
			}
		}
	}
}

static void CancelOneOrderFromBinance() {
	string symbol = _BinancePairSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (symbol.size() != 0) {
		long orderId = _GetLongValue("Enter orderId:");
		if (orderId > 0)
			binance->CancelOrder(symbol, orderId);
	}
}

static void CancelAllOrdersFromBinance() {
	string symbol = _BinancePairSubMenuList(WITH_ALL);
	if (symbol.size() != 0)
		BotMethod::CancelAllOrders(symbol, "BINANCE");
}

//-----------------
// Ramzinex
//-----------------
static void ShowRamzinexPrice() {
	int choice;

	map <int, string> RamzinexPriceMenuOption;
	RamzinexPriceMenuOption.insert(pair <int, string> (1, "All"));
	RamzinexPriceMenuOption.insert(pair <int, string> (2, "WatchList"));
	RamzinexPriceMenuOption.insert(pair <int, string> (3, "USDTIRR"));
	RamzinexPriceMenuOption.insert(pair <int, string> (4, "XRPIRR"));
	RamzinexPriceMenuOption.insert(pair <int, string> (5, "BTCIRR"));
	RamzinexPriceMenuOption.insert(pair <int, string> (6, "ETHIRR"));
	RamzinexPriceMenuOption.insert(pair <int, string> (7, "LTCIRR"));
	RamzinexPriceMenuOption.insert(pair <int, string> (8, "XRPUSDT"));
	RamzinexPriceMenuOption.insert(pair <int, string> (9, "BTCUSDT"));
	RamzinexPriceMenuOption.insert(pair <int, string> (10, "ETHUSDT"));
	RamzinexPriceMenuOption.insert(pair <int, string> (11, "LTCUSDT"));
	RamzinexPriceMenuOption.insert(pair <int, string> (12, "DOGEIRR"));

	cout << "Select from following menu:\n";

	map<int, string>::iterator it;
	for (it = RamzinexPriceMenuOption.begin(); it != RamzinexPriceMenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;

	it = RamzinexPriceMenuOption.find(choice);
	if (it != RamzinexPriceMenuOption.end())
		Ramzinex::ShowPrices(it->second);
	else 			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;
}

//-----------------
// Coinex
//-----------------
static void ShowCoinexPrice() {
	// cout << "Inside ShowCoinexPrice\n";

	string SubMenuSelection = _CoinexPairSubMenuList(WITH_WATCHLIST);
	// cout << "ShowCoinexPrice: " << SubMenuSelection << endl;
	if (SubMenuSelection.size() != 0)
		coinex->ShowPrices(SubMenuSelection);
}

static void ShowCoinexOpenOrders() {
	// cout << "Inside ShowCoinexOpenOrders\n";

	string SubMenuSelection = _CoinexPairSubMenuList(WITH_WATCHLIST);
	if (SubMenuSelection.size() != 0)
		coinex->ShowOpenOrders(SubMenuSelection);
}

static void ShowCoinexMyTrades () {
	// cout << "Inside ShowCoinexMyTrades\n";

	string SubMenuSelection = _CoinexPairSubMenuList(WITH_WATCHLIST);
	if (SubMenuSelection.size() != 0) {
		int limit = 20;
		// cout << "By default last " << limit << " trades are shown\n";
		int pastDay = _DurationPerDay();
		if (pastDay >= 0)
			coinex->ShowMyTrades(SubMenuSelection, pastDay); // limit); 
	}
}

static void ShowCoinexTradesPerformance () {
	cout << "Inside ShowCoinexTradesPerformance\n";

	string SubMenuSelection = _CoinexPairSubMenuList(WITH_WATCHLIST);
	if (SubMenuSelection.size() != 0) {
		int pastDay = _DurationPerDay();
		if (pastDay >= 0)
			coinex->ShowTradesPerformance(SubMenuSelection, pastDay);	
	}
}

static void ShowCoinexDepositHistory () {
	// cout << "Inside ShowCoinexDepositHistory\n";

	string SubMenuSelection = _CoinexSingleSubMenuList(WITH_ALL);
	if (SubMenuSelection.size() != 0)
		coinex->ShowDepositHistory(SubMenuSelection);	
}

static void ShowCoinexWithdrawHistory () {
	cout << "Inside ShowCoinexWithdrawHistory\n";

	string SubMenuSelection = _CoinexSingleSubMenuList(WITH_ALL);
	if (SubMenuSelection.size() != 0)
		coinex->ShowWithdrawHistory(SubMenuSelection);	
}

static void SendOrderToCoinex() {
	string symbol = _CoinexPairSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (symbol.size() != 0) {
		string side = _OrderSideList();
		if (side.size() != 0) {
			string type = _CoinexOrderTypeList();
			if (type.size() != 0) {
				double quantity = _GetDoubleValue("Enter the quantity:");
				if (quantity > 0) {
					if (type != "MARKET") {
						double price = _GetDoubleValue("Enter the price:");
						if (price > 0)
							coinex->SendOrder(symbol, side, type, quantity, price);
					}
					else 
						coinex->SendOrder(symbol, side, type, quantity, 0);
				}
			}
		}
	}
}

static void CancelOneOrderFromCoinex() {
	string symbol = _CoinexPairSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (symbol.size() != 0) {
		long orderId = _GetLongValue("Enter orderId:");
		if (orderId > 0)
			coinex->CancelOrder(symbol, orderId);
	}
}

static void CancelAllOrdersFromCoinex() {
	string symbol = _CoinexPairSubMenuList(WITH_WATCHLIST);
	if (symbol.size() != 0)
		BotMethod::CancelAllOrders(symbol, "COINEX");
}

//-----------------
// HitBTC
//-----------------
static void ShowHitBTCPrice() {
	// cout << "Inside ShowHitBTCPrice\n";

	string SubMenuSelection = _HitBTCPairSubMenuList(WITH_ALL_AND_WATCHLIST);
	// cout << "ShowHitBTCPrice: " << SubMenuSelection << endl;
	if (SubMenuSelection.size() != 0)
		hitbtc->ShowPrices(SubMenuSelection);
}

static void ShowHitBTCOpenOrders() {
	// cout << "Inside ShowHitBTCOpenOrders\n";

	string SubMenuSelection = _HitBTCPairSubMenuList(WITH_ALL);
	if (SubMenuSelection.size() != 0)
		hitbtc->ShowOpenOrders(SubMenuSelection);
}

static void ShowHitBTCMyTrades() {
	// cout << "Inside ShowHitBTCMyTrades\n";

	string SubMenuSelection = _HitBTCPairSubMenuList(WITH_WATCHLIST);
	if (SubMenuSelection.size() != 0) {
		int pastDay = _DurationPerDay();
		if (pastDay >= 0)
			hitbtc->ShowMyTrades(SubMenuSelection, pastDay);	
	}
}

static void ShowHitBTCTradesPerformance () {
	// cout << "Inside ShowHitBTCTradesPerformance\n";

	string SubMenuSelection = _HitBTCPairSubMenuList(WITH_WATCHLIST);
	if (SubMenuSelection.size() != 0) {
		int pastDay = _DurationPerDay();
		if (pastDay >= 0)
			hitbtc->ShowTradesPerformance(SubMenuSelection, pastDay);	
	}
}

static void ShowHitBTCDepositAddress () {
	// cout << "Inside ShowHitBTCDepositAddress\n";

	string SubMenuSelection = _HitBTCSingleSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (SubMenuSelection.size() != 0)
		hitbtc->ShowDepositAddress(SubMenuSelection);
}

static void SendOrderToHitBTC() {
	string symbol = _HitBTCPairSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (symbol.size() != 0) {
		string side = _OrderSideList();
		if (side.size() != 0) {
			string type = _HitBTCOrderTypeList();
			if (type.size() != 0) {
				double quantity = _GetDoubleValue("Enter the quantity:");
				if (quantity > 0) {
					if (type != "market") {
						double price = _GetDoubleValue("Enter the price:");
						if (price > 0)
							hitbtc->SendOrder(symbol, side, type, quantity, price);
					}
					else 
						hitbtc->SendOrder(symbol, side, type, quantity, 0);
				}
			}
		}
	}
}

static void CancelOneOrderFromHitBTC() {
	string symbol = _HitBTCPairSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (symbol.size() != 0) {
		string clientOrderId = _GetStringValue("Enter clientOrderId:");
		if (clientOrderId.size() != 0)
			hitbtc->CancelOrder(symbol, clientOrderId);
	}
}

static void CancelAllOrdersFromHitBTC() {
	string symbol = _HitBTCPairSubMenuList(WITH_ALL);
	if (symbol.size() != 0)
		BotMethod::CancelAllOrders(symbol, "HITBTC");
}

static void TransferBetweenBankAndExchangeInHitBTC() {
	string currency = _HitBTCSingleSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (currency.size() != 0) {
		double amount = _GetDoubleValue("Enter the amount:");
		if (amount > 0) {
			string type = _HitBTCTransferType();
			if (type.size() != 0)
				hitbtc->TransferBetweenBankAndExchange(currency, amount, type);
		}
	}
}

//-----------------
// Kucoin
//-----------------
static void ShowKucoinPrice() {
	// cout << "Inside ShowKucoinPrice\n";

	string SubMenuSelection = _KucoinPairSubMenuList(WITH_ALL_AND_WATCHLIST);
	// cout << "ShowKucoinPrice: " << SubMenuSelection << endl;
	if (SubMenuSelection.size() != 0)
		kucoin->ShowPrices(SubMenuSelection);
}

static void ShowKucoinOpenOrders() {
	// cout << "Inside ShowKucoinOpenOrders\n";

	string SubMenuSelection = _KucoinPairSubMenuList(WITH_ALL);
	if (SubMenuSelection.size() != 0)
		kucoin->ShowOpenOrders(SubMenuSelection);
}

static void ShowKucoinMyTrades() {
	// cout << "Inside ShowKucoinMyTrades\n";

	string SubMenuSelection = _KucoinPairSubMenuList(WITH_ALL_AND_WATCHLIST);
	if (SubMenuSelection.size() != 0) {
		int pastDay = _DurationPerDay();
		if (pastDay >= 0)
			kucoin->ShowMyTrades(SubMenuSelection, pastDay);	
	}
}

static void ShowKucoinTradesPerformance () {
	// cout << "Inside ShowKucoinTradesPerformance\n";

	string SubMenuSelection = _KucoinPairSubMenuList(WITH_WATCHLIST);
	if (SubMenuSelection.size() != 0) {
		int pastDay = _DurationPerDay();
		if (pastDay >= 0)
			kucoin->ShowTradesPerformance(SubMenuSelection, pastDay);	
	}
}

static void ShowKucoinDepositAddress () {
	// cout << "Inside ShowKucoinDepositAddress\n";

	string SubMenuSelection = _KucoinSingleSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (SubMenuSelection.size() != 0)
		kucoin->ShowDepositAddress(SubMenuSelection);
}

static void ShowKucoinDepositHistory () {
	// cout << "Inside ShowKucoinDepositHistory\n";

	string SubMenuSelection = _KucoinSingleSubMenuList(WITH_ALL);
	if (SubMenuSelection.size() != 0)
		kucoin->ShowDepositHistory(SubMenuSelection);	
}

static void ShowKucoinWithdrawHistory () {
	// cout << "Inside ShowKucoinWithdrawHistory\n";

	string SubMenuSelection = _KucoinSingleSubMenuList(WITH_ALL);
	if (SubMenuSelection.size() != 0)
		kucoin->ShowWithdrawHistory(SubMenuSelection);	
}

static void SendOrderToKucoin() {
	string symbol = _KucoinPairSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (symbol.size() != 0) {
		string side = _OrderSideList();
		if (side.size() != 0) {
			string type = _KucoinOrderTypeList();
			if (type.size() != 0) {
				double quantity = _GetDoubleValue("Enter the quantity:");
				if (quantity > 0) {
					if (type != "market") {
						double price = _GetDoubleValue("Enter the price:");
						if (price > 0)
							kucoin->SendOrder(symbol, side, type, quantity, price);
					}
					else 
						kucoin->SendOrder(symbol, side, type, quantity, 0);
				}
			}
		}
	}
}

static void CancelOneOrderFromKucoin() {
	string symbol = _KucoinPairSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (symbol.size() != 0) {
		string orderId = _GetStringValue("Enter orderId:");
		if (orderId.size() != 0)
			kucoin->CancelOrder(symbol, orderId);
	}
}

static void CancelAllOrdersFromKucoin() {
	string symbol = _KucoinPairSubMenuList(WITH_ALL);
	if (symbol.size() != 0)
		BotMethod::CancelAllOrders(symbol, "KUCOIN");
}

static void TransferBetweenBankAndExchangeInKucoin() {
	string currency = _KucoinSingleSubMenuList(WITHOUT_ALL_AND_WATCHLIST);
	if (currency.size() != 0) {
		double amount = _GetDoubleValue("Enter the amount:");
		if (amount > 0) {
			string type = _KucoinTransferType();
			if (type.size() != 0)
				kucoin->TransferBetweenBankAndExchange(currency, amount, type);
		}
	}
}

//-----------------
// Old style Main menu
//-----------------

// void Menu::BinanceMenu() {
// 	int choice;

// 	map <int, MainMenuMapValues> MapMenuOption;
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (0, MainMenuMapValues("Exit", & Exit)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (1, MainMenuMapValues("ShowPriceFromBinance", & ShowBinancePrice)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (2, MainMenuMapValues("ShowBinanceBalances", & binance->ShowBalances)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (3, MainMenuMapValues("ShowBinanceBalanceInUSDT", & binance->ShowBalanceInUSDT)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (4, MainMenuMapValues("ShowBinanceOpenOrders", & ShowBinanceOpenOrders)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (5, MainMenuMapValues("ShowBinanceAllOrders", & ShowBinanceAllOrders)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (6, MainMenuMapValues("ShowBinanceMyTrades", & ShowBinanceMyTrades)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (7, MainMenuMapValues("ShowBinanceTradesPerformance", & ShowBinanceTradesPerformance)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (8, MainMenuMapValues("ShowBinanceDepositAddress", & ShowBinanceDepositAddress)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (9, MainMenuMapValues("ShowBinanceDepositHistory", & ShowBinanceDepositHistory)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (10, MainMenuMapValues("ShowBinanceWithdrawHistory", & ShowBinanceWithdrawHistory)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (11, MainMenuMapValues("CancelAllOrdersFromBinance", & CancelAllOrdersFromBinance)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (12, MainMenuMapValues("SendOrderBinance", & SendOrderToBinance)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (13, MainMenuMapValues("CancelOrderBinance", & CancelOneOrderFromBinance)));

// 	cout << "Select from following menu:\n";

// 	map<int, MainMenuMapValues>::iterator it;
// 	for (it = MapMenuOption.begin(); it != MapMenuOption.end(); it++)
// 		cout << "\t" << it->first << ") " << it->second.description << endl;
// 	cout << KGRN << "\nYour choice:  \n" << RESET;
// 	cin >> choice;
// 	// cout << "choice: " << KGRN << choice << RESET << endl;

// 	it = MapMenuOption.find(choice);
// 	if (it != MapMenuOption.end())
// 		MapMenuOption[choice].method();
// 	else			// invlid number
// 		cout << KRED << "Invalid number\n\n\n" << RESET;
// }

// void Menu::RamzinexMenu() {
// 	int choice;

// 	map <int, MainMenuMapValues> MapMenuOption;
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (0, MainMenuMapValues("Exit", & Exit)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (1, MainMenuMapValues("ShowPriceFromRamzinex", & ShowRamzinexPrice)));
	
// 	cout << "Select from following menu:\n";

// 	map<int, MainMenuMapValues>::iterator it;
// 	for (it = MapMenuOption.begin(); it != MapMenuOption.end(); it++)
// 		cout << "\t" << it->first << ") " << it->second.description << endl;
// 	cout << KGRN << "\nYour choice:  \n" << RESET;
// 	cin >> choice;
// 	// cout << "choice: " << KGRN << choice << RESET << endl;

// 	it = MapMenuOption.find(choice);
// 	if (it != MapMenuOption.end())
// 		MapMenuOption[choice].method();
// 	else			// invlid number
// 		cout << KRED << "Invalid number\n\n\n" << RESET;	
// }

// void Menu::CoinexMenu() {
// 	int choice;

// 	map <int, MainMenuMapValues> MapMenuOption;
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (0, MainMenuMapValues("Exit", & Exit)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (1, MainMenuMapValues("ShowPriceFromCoinex", & ShowCoinexPrice)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (2, MainMenuMapValues("ShowCoinexBalances", & coinex->ShowBalances)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (3, MainMenuMapValues("ShowCoinexBalanceInUSDT", & coinex->ShowBalanceInUSDT)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (4, MainMenuMapValues("ShowCoinexOpenOrders", & ShowCoinexOpenOrders)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (6, MainMenuMapValues("ShowBinanceMyTrades", & ShowCoinexMyTrades)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (9, MainMenuMapValues("ShowCoinexDepositHistory", & ShowCoinexDepositHistory)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (10, MainMenuMapValues("ShowCoinexWithdrawHistory", & ShowCoinexWithdrawHistory)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (11, MainMenuMapValues("CancelAllOrdersFromCoinex", & CancelAllOrdersFromCoinex)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (12, MainMenuMapValues("SendOrderCoinex", & SendOrderToCoinex)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (13, MainMenuMapValues("CancelOrderCoinex", & CancelOneOrderFromCoinex)));


// 	cout << "Select from following menu:\n";

// 	map<int, MainMenuMapValues>::iterator it;
// 	for (it = MapMenuOption.begin(); it != MapMenuOption.end(); it++)
// 		cout << "\t" << it->first << ") " << it->second.description << endl;
// 	cout << KGRN << "\nYour choice:  \n" << RESET;
// 	cin >> choice;
// 	// cout << "choice: " << KGRN << choice << RESET << endl;

// 	it = MapMenuOption.find(choice);
// 	if (it != MapMenuOption.end())
// 		MapMenuOption[choice].method();
// 	else			// invlid number
// 		cout << KRED << "Invalid number\n\n\n" << RESET;	
// }

// void Menu::HitBTCMenu() {
// 	int choice;

// 	map <int, MainMenuMapValues> MapMenuOption;
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (0, MainMenuMapValues("Exit", & Exit)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (1, MainMenuMapValues("ShowPriceFromHitBTC", & ShowHitBTCPrice)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (2, MainMenuMapValues("ShowHitBTCBalances", & hitbtc->ShowBalances)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (3, MainMenuMapValues("ShowHitBTCBalanceInUSDT", & hitbtc->ShowBalanceInUSDT)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (4, MainMenuMapValues("ShowHitBTCOpenOrders", & ShowHitBTCOpenOrders)));
// 	// MapMenuOption.insert(pair <int, MainMenuMapValues> (5, MainMenuMapValues("ShowHitBTCAllOrders", & ShowHitBTCAllOrders)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (6, MainMenuMapValues("ShowHitBTCMyTrades", & ShowHitBTCMyTrades)));
// 	// MapMenuOption.insert(pair <int, MainMenuMapValues> (7, MainMenuMapValues("ShowHitBTCTradesPerformance", & ShowHitBTCTradesPerformance)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (8, MainMenuMapValues("ShowHitBTCDepositAddress", & ShowHitBTCDepositAddress)));
// 	// MapMenuOption.insert(pair <int, MainMenuMapValues> (9, MainMenuMapValues("ShowHitBTCDepositHistory", & ShowHitBTCDepositHistory)));
// 	// MapMenuOption.insert(pair <int, MainMenuMapValues> (10, MainMenuMapValues("ShowHitBTCWithdrawHistory", & ShowHitBTCWithdrawHistory)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (11, MainMenuMapValues("CancelAllOrdersFromHitBTC", & CancelAllOrdersFromHitBTC)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (12, MainMenuMapValues("SendOrderHitBTC", & SendOrderToHitBTC)));
// 	MapMenuOption.insert(pair <int, MainMenuMapValues> (13, MainMenuMapValues("CancelOrderHitBTC", & CancelOneOrderFromHitBTC)));

// 	cout << "Select from following menu:\n";

// 	map<int, MainMenuMapValues>::iterator it;
// 	for (it = MapMenuOption.begin(); it != MapMenuOption.end(); it++)
// 		cout << "\t" << it->first << ") " << it->second.description << endl;
// 	cout << KGRN << "\nYour choice:  \n" << RESET;
// 	cin >> choice;
// 	// cout << "choice: " << KGRN << choice << RESET << endl;

// 	it = MapMenuOption.find(choice);
// 	if (it != MapMenuOption.end())
// 		MapMenuOption[choice].method();
// 	else			// invlid number
// 		cout << KRED << "Invalid number\n\n\n" << RESET;	
// }

void Menu::BotMenu() {
	int choice;

	map <int, MainMenuMapValues> MapMenuOption;
	MapMenuOption.insert(pair <int, MainMenuMapValues> (0, MainMenuMapValues("Exit", & Exit)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (1, MainMenuMapValues("BotDollarPriceInRamzinex", & BotMethod::DollarPriceInRamzinex)));
	// MapMenuOption.insert(pair <int, MainMenuMapValues> (2, MainMenuMapValues("BotCheckArbitrage", & BotMethod::CheckArbitrage)));
	// MapMenuOption.insert(pair <int, MainMenuMapValues> (3, MainMenuMapValues("BotScalping", & BotMethod::Scalping)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (4, MainMenuMapValues("BotHodling", & BotMethod::Hodling)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (5, MainMenuMapValues("BotTrading", & BotMethod::Trading)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (6, MainMenuMapValues("BotHodlingTrading", & BotMethod::HodlingTrading)));
	// MapMenuOption.insert(pair <int, MainMenuMapValues> (5, MainMenuMapValues("BotScalping2", & BotMethod::Scalping2)));
	// MapMenuOption.insert(pair <int, MainMenuMapValues> (6, MainMenuMapValues("SetJsonFileOrders", & BotMethod::SetJsonFileOrders)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (7, MainMenuMapValues("CheckHistoryHodling", & BotMethod::CheckHistoryHodling)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (8, MainMenuMapValues("CheckHistoryTrading", & BotMethod::CheckHistoryTrading)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (9, MainMenuMapValues("CheckHistoryHodlingTrading", & BotMethod::CheckHistoryHodlingTrading)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (22, MainMenuMapValues("RandomBuyFromBinance", & BotMethod::RandomBuy)));

	cout << "Select from following menu:\n";

	map<int, MainMenuMapValues>::iterator it;
	for (it = MapMenuOption.begin(); it != MapMenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second.description << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;

	it = MapMenuOption.find(choice);
	if (it != MapMenuOption.end())
		MapMenuOption[choice].method();
	else			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;	
}

void Menu::UtilityMenu() {
	int choice;

	map <int, MainMenuMapValues> MapMenuOption;
	MapMenuOption.insert(pair <int, MainMenuMapValues> (0, MainMenuMapValues("Exit", & Exit)));
	// MapMenuOption.insert(pair <int, MainMenuMapValues> (1, MainMenuMapValues("TestBinanceConnectivity", & binance->TestConnectivity)));
	// MapMenuOption.insert(pair <int, MainMenuMapValues> (2, MainMenuMapValues("ShowBinanceServerTime", & binance->ShowServerTime)));
	// MapMenuOption.insert(pair <int, MainMenuMapValues> (3, MainMenuMapValues("ShowBinanceAccountStatus", & binance->ShowAccountStatus)));
	// MapMenuOption.insert(pair <int, MainMenuMapValues> (4, MainMenuMapValues("ShowBinanceExchangeInfo", & binance->ShowExchangeInfo)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (5, MainMenuMapValues("ShowMyExternalIpAddress", & CurlAPI::ShowExternalIpAddress)));
	// MapMenuOption.insert(pair <int, MainMenuMapValues> (12, MainMenuMapValues("ShowKucoinServerTime", & kucoin->ShowServerTime)));

	cout << "Select from following menu:\n";

	map<int, MainMenuMapValues>::iterator it;
	for (it = MapMenuOption.begin(); it != MapMenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second.description << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;

	it = MapMenuOption.find(choice);
	if (it != MapMenuOption.end())
		MapMenuOption[choice].method();
	else			// invlid number
		cout << KRED << "Invalid number\n\n\n" << RESET;
}

//-----------------
// New Main menu
//-----------------
void Menu::ShowPrice() {
	int mode = ALL | BINANCE | RAMZINEX | COINEX | HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & ALL) && symbol == "All")
			ShowAllPrice(mode);
		if ((mode & BINANCE) && symbol == "Binance")
			ShowBinancePrice();
		if ((mode & RAMZINEX) && symbol == "Ramzinex")
			ShowRamzinexPrice();
		if ((mode & COINEX) && symbol == "Coinex")
			ShowCoinexPrice();
		if ((mode & HITBTC) && symbol == "HitBTC")
			ShowHitBTCPrice();
		if ((mode & KUCOIN) && symbol == "Kucoin")
			ShowKucoinPrice();
	}
}

void Menu::ShowBalances() {
	int mode = ALL | BINANCE | COINEX | HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & BINANCE) &&  (symbol == "All" || symbol == "Binance"))
			binance->ShowBalances();
		if ((mode & RAMZINEX) &&  (symbol == "All" || symbol == "Ramzinex"))
			_NotSupported();
		if ((mode & COINEX) &&  (symbol == "All" || symbol == "Coinex"))
			coinex->ShowBalances();
		if ((mode & HITBTC) &&  (symbol == "All" || symbol == "HitBTC"))
			hitbtc->ShowBalances();
		if ((mode & KUCOIN) &&  (symbol == "All" || symbol == "Kucoin"))
			kucoin->ShowBalances();
	}
}

void Menu::ShowBalanceInUSDT() {
	int mode = ALL | BINANCE | COINEX | HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & ALL) && symbol == "All")
			BotMethod::ShowBalanceInUSDT();
		if ((mode & BINANCE) && symbol == "Binance")
			binance->ShowBalanceInUSDT();
		if ((mode & RAMZINEX) && symbol == "Ramzinex")
			_NotSupported();
		if ((mode & COINEX) && symbol == "Coinex")
			coinex->ShowBalanceInUSDT();
		if ((mode & HITBTC) && symbol == "HitBTC")
			hitbtc->ShowBalanceInUSDT();
		if ((mode & KUCOIN) && symbol == "Kucoin")
			kucoin->ShowBalanceInUSDT();
	}
}

void Menu::ShowOpenOrders() {
	int mode = ALL | BINANCE | COINEX | HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & ALL) && symbol == "All")
			ShowAllOpenOrders(mode);
		if ((mode & BINANCE) && symbol == "Binance")
			ShowBinanceOpenOrders();
		if ((mode & RAMZINEX) && symbol == "Ramzinex")
			_NotSupported();
		if ((mode & COINEX) && symbol == "Coinex")
			ShowCoinexOpenOrders();
		if ((mode & HITBTC) && symbol == "HitBTC")
			ShowHitBTCOpenOrders();
		if ((mode & KUCOIN) && symbol == "Kucoin")
			ShowKucoinOpenOrders();
	}
}

void Menu::ShowAllOrders() {
	int mode = ALL | BINANCE;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & BINANCE) &&  (symbol == "All" || symbol == "Binance"))
			ShowBinanceAllOrders();
		if ((mode & RAMZINEX) &&  (symbol == "All" || symbol == "Ramzinex"))
			_NotSupported();
		if ((mode & COINEX) &&  (symbol == "All" || symbol == "Coinex"))
			_NotSupported();
		if ((mode & HITBTC) &&  (symbol == "All" || symbol == "HitBTC"))
			_NotSupported();
	}
}

void Menu::ShowMyTrades() {
	int mode = ALL | BINANCE | COINEX | HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & ALL) && symbol == "All")
			ShowAllMyTrades(mode);
		if ((mode & BINANCE) && symbol == "Binance")
			ShowBinanceMyTrades();
		if ((mode & RAMZINEX) && symbol == "Ramzinex")
			_NotSupported();
		if ((mode & COINEX) && symbol == "Coinex")
			ShowCoinexMyTrades();
		if ((mode & HITBTC) && symbol == "HitBTC")
			ShowHitBTCMyTrades();
		if ((mode & KUCOIN) && symbol == "Kucoin")
			ShowKucoinMyTrades();
	}
}

void Menu::ShowTradesPerformance() {
	int mode = BINANCE | COINEX | HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & BINANCE) && (symbol == "All" || symbol == "Binance"))
			ShowBinanceTradesPerformance();
		if ((mode & RAMZINEX) && (symbol == "All" || symbol == "Ramzinex"))
			_NotSupported();
		if ((mode & COINEX) && (symbol == "All" || symbol == "Coinex"))
			ShowCoinexTradesPerformance();
		if ((mode & HITBTC) && (symbol == "All" || symbol == "HitBTC"))
			ShowHitBTCTradesPerformance();
		if ((mode & KUCOIN) && (symbol == "All" || symbol == "Kucoin"))
			ShowKucoinTradesPerformance();
	}
}

void Menu::ShowDepositAddress() {
	int mode = BINANCE | HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & ALL) && symbol == "All")
			_NotSupported();
		if ((mode & BINANCE) && symbol == "Binance")
			ShowBinanceDepositAddress();
		if ((mode & RAMZINEX) && symbol == "Ramzinex")
			_NotSupported();
		if ((mode & COINEX) && symbol == "Coinex")
			_NotSupported();
		if ((mode & HITBTC) && symbol == "HitBTC")
			ShowHitBTCDepositAddress();
		if ((mode & KUCOIN) && symbol == "Kucoin")
			ShowKucoinDepositAddress();
	}
}

void Menu::ShowDepositHistory() {
	int mode = ALL | BINANCE | COINEX | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & ALL) && symbol == "All")
			ShowAllDepositHistory(mode);
		if ((mode & BINANCE) && symbol == "Binance")
			ShowBinanceDepositHistory();
		if ((mode & RAMZINEX) && symbol == "Ramzinex")
			_NotSupported();
		if ((mode & COINEX) && symbol == "Coinex")
			ShowCoinexDepositHistory();
		if ((mode & HITBTC) && symbol == "HitBTC")
			_NotSupported();
		if ((mode & KUCOIN) && symbol == "Kucoin")
			ShowKucoinDepositHistory();
	}
}

void Menu::ShowWithdrawHistory() {
	int mode = ALL | BINANCE | COINEX | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & ALL) && symbol == "All")
			ShowAllWithdrawHistory(mode);
		if ((mode & BINANCE) && symbol == "Binance")
			ShowBinanceWithdrawHistory();
		if ((mode & RAMZINEX) && symbol == "Ramzinex")
			_NotSupported();
		if ((mode & COINEX) && symbol == "Coinex")
			ShowCoinexWithdrawHistory();
		if ((mode & HITBTC) && symbol == "HitBTC")
			_NotSupported();
		if ((mode & KUCOIN) && symbol == "Kucoin")
			ShowKucoinWithdrawHistory();
	}
}

void Menu::CancelAllOrders() {
	int mode = BINANCE | COINEX | HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & ALL) && symbol == "All")
			_NotSupported();
		if ((mode & BINANCE) && symbol == "Binance")
			CancelAllOrdersFromBinance();
		if ((mode & RAMZINEX) && symbol == "Ramzinex")
			_NotSupported();
		if ((mode & COINEX) && symbol == "Coinex")
			CancelAllOrdersFromCoinex();
		if ((mode & HITBTC) && symbol == "HitBTC")
			CancelAllOrdersFromHitBTC();
		if ((mode & KUCOIN) && symbol == "Kucoin")
			CancelAllOrdersFromKucoin();
	}
}

void Menu::SendOrder() {
	int mode = BINANCE | COINEX | HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & ALL) && symbol == "All")
			_NotSupported();
		if ((mode & BINANCE) && symbol == "Binance")
			SendOrderToBinance();
		if ((mode & RAMZINEX) && symbol == "Ramzinex")
			_NotSupported();
		if ((mode & COINEX) && symbol == "Coinex")
			SendOrderToCoinex();
		if ((mode & HITBTC) && symbol == "HitBTC")
			SendOrderToHitBTC();
		if ((mode & KUCOIN) && symbol == "Kucoin")
			SendOrderToKucoin();
	}
}

void Menu::CancelOrder() {
	int mode = BINANCE | COINEX | HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & ALL) && symbol == "All")
			_NotSupported();
		if ((mode & BINANCE) && symbol == "Binance")
			CancelOneOrderFromBinance();
		if ((mode & RAMZINEX) && symbol == "Ramzinex")
			_NotSupported();
		if ((mode & COINEX) && symbol == "Coinex")
			CancelOneOrderFromCoinex();
		if ((mode & HITBTC) && symbol == "HitBTC")
			CancelOneOrderFromHitBTC();
		if ((mode & KUCOIN) && symbol == "Kucoin")
			CancelOneOrderFromKucoin();
	}
}

void Menu::ShowBankBalances() {
	int mode = ALL | HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & BINANCE) &&  (symbol == "All" || symbol == "Binance"))
			_NotSupported();
		if ((mode & RAMZINEX) &&  (symbol == "All" || symbol == "Ramzinex"))
			_NotSupported();
		if ((mode & COINEX) &&  (symbol == "All" || symbol == "Coinex"))
			_NotSupported();
		if ((mode & HITBTC) &&  (symbol == "All" || symbol == "HitBTC"))
			hitbtc->ShowBankBalances();
		if ((mode & KUCOIN) &&  (symbol == "All" || symbol == "Kucoin"))
			kucoin->ShowBankBalances();
	}
}

void Menu::ShowExchangeBalances() {
	int mode = ALL | HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & BINANCE) &&  (symbol == "All" || symbol == "Binance"))
			_NotSupported();
		if ((mode & RAMZINEX) &&  (symbol == "All" || symbol == "Ramzinex"))
			_NotSupported();
		if ((mode & COINEX) &&  (symbol == "All" || symbol == "Coinex"))
			_NotSupported();
		if ((mode & HITBTC) &&  (symbol == "All" || symbol == "HitBTC"))
			hitbtc->ShowExchangeBalances();
		if ((mode & KUCOIN) &&  (symbol == "All" || symbol == "Kucoin"))
			kucoin->ShowExchangeBalances();
	}
}

void Menu::TransferBetweenBankAndExchange() {
	int mode = HITBTC | KUCOIN;
	string symbol = _ExchangeSubMenuList(mode);

	if (symbol.size() != 0) {
		if ((mode & ALL) && symbol == "All")
			_NotSupported();
		if ((mode & BINANCE) && symbol == "Binance")
			_NotSupported();
		if ((mode & RAMZINEX) && symbol == "Ramzinex")
			_NotSupported();
		if ((mode & COINEX) && symbol == "Coinex")
			_NotSupported();
		if ((mode & HITBTC) && symbol == "HitBTC")
			TransferBetweenBankAndExchangeInHitBTC();
		if ((mode & KUCOIN) && symbol == "Kucoin")
			TransferBetweenBankAndExchangeInKucoin();
	}
}


int Menu::AskAboutNewMenu() {
	int c;
	cout << KGRN << "Press 0 to Exit or other keys to continue ...\n" << RESET;
	cin >> c;
	switch (c) {
		case 0:
			return -1;
		default: 
			return 0;
	}
}

int Menu::MainMenu() {
	int choice;

	// reset progress bar counter

	map <int, MainMenuMapValues> MapMenuOption;
	MapMenuOption.insert(pair <int, MainMenuMapValues> (0, MainMenuMapValues("Exit", & Exit)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (1, MainMenuMapValues("ShowPrice", & ShowPrice)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (2, MainMenuMapValues("ShowBalances", & ShowBalances)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (3, MainMenuMapValues("ShowBalanceInUSDT", & ShowBalanceInUSDT)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (4, MainMenuMapValues("ShowOpenOrders", & ShowOpenOrders)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (5, MainMenuMapValues("ShowAllOrders", & ShowAllOrders)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (6, MainMenuMapValues("ShowMyTrades", & ShowMyTrades)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (7, MainMenuMapValues("ShowTradesPerformance", & ShowTradesPerformance)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (8, MainMenuMapValues("ShowDepositAddress", & ShowDepositAddress)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (9, MainMenuMapValues("ShowDepositHistory", & ShowDepositHistory)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (10, MainMenuMapValues("ShowWithdrawHistory", & ShowWithdrawHistory)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (11, MainMenuMapValues("CancelAllOrders", & CancelAllOrders)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (12, MainMenuMapValues("SendOrder", & SendOrder)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (13, MainMenuMapValues("CancelOrder", & CancelOrder)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (14, MainMenuMapValues("ShowBankBalances", & ShowBankBalances)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (15, MainMenuMapValues("ShowExchangeBalances", & ShowExchangeBalances)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (16, MainMenuMapValues("TransferBetweenBankAndExchange", & TransferBetweenBankAndExchange)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (88, MainMenuMapValues("Bot", & BotMenu)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (99, MainMenuMapValues("Utility", & UtilityMenu)));
	MapMenuOption.insert(pair <int, MainMenuMapValues> (111, MainMenuMapValues("Test", & BotMethod::Test)));

	cout << "Select from following menu:\n";

	map<int, MainMenuMapValues>::iterator it;
	for (it = MapMenuOption.begin(); it != MapMenuOption.end(); it++)
		cout << "\t" << it->first << ") " << it->second.description << endl;
	cout << KGRN << "\nYour choice:  \n" << RESET;
	cin >> choice;
	// cout << "choice: " << KGRN << choice << RESET << endl;

	it = MapMenuOption.find(choice);
	if (it != MapMenuOption.end()) {
		CurlAPI::ResetProgressCnt();
		MapMenuOption[choice].method();
		return AskAboutNewMenu();
	}
	else {			// invalid number
		cout << KRED << "Invalid number\n\n\n" << RESET;
		return -1;
	}
}

void Menu::Init() {
	binance = Binance::getInstance();
	coinex = Coinex::getInstance();
	hitbtc = HitBTC::getInstance();
	kucoin = Kucoin::getInstance();
}

void Menu::Cleanup() {
	// cout << "Successfully perform Menu cleaning up\n";
}
