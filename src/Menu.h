#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <exception>

#include <curl/curl.h>
#include <json/json.h>


using namespace std;


struct MainMenuMapValues {
	string description;
	void (*method) ();
	MainMenuMapValues(string s, void (*f) ()) : description(s), method (f) {}
	MainMenuMapValues() {}
};

struct DayDurationMapValues {
	string description;
	int Day;
	DayDurationMapValues(string s, int d) : description(s), Day(d) {}
	DayDurationMapValues() {}
};

class Menu
{
private:
	static void UtilityMenu();
	// static void BinanceMenu();
	// static void RamzinexMenu();
	// static void CoinexMenu();
	// static void HitBTCMenu();
	static void BotMenu();
	static int AskAboutNewMenu();

	static void ShowPrice();
	static void ShowBalances();
	static void ShowBalanceInUSDT();
	static void ShowOpenOrders();
	static void ShowAllOrders();
	static void ShowMyTrades();
	static void ShowTradesPerformance();
	static void ShowDepositAddress();
	static void ShowDepositHistory();
	static void ShowWithdrawHistory();
	static void CancelAllOrders();
	static void SendOrder();
	static void CancelOrder();
	static void ShowBankBalances();
	static void ShowExchangeBalances();
	static void TransferBetweenBankAndExchange();

public:
	static int MainMenu();
};

#endif