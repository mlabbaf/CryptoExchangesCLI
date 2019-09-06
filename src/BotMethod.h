#ifndef BOTMETHOD_H
#define BOTMETHOD_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <exception>
#include <list>
#include <set>
#include <algorithm>
#include <functional>
#include <iomanip>

#include <curl/curl.h>
#include <json/json.h>

#include "Definitions.h"


using namespace std;


struct StructDifferentExchangePrices {
	double BinancePrice;
	double RamzinexSellPrice;
	double RamzinexBuyPrice;
};

struct OrderParams {
	string pair;
	string side;
	string type;
	double price;
	double stopPrice;
	double origQty;
	long orderId;
};

struct HodlingParams {
	string symbol;
	string pair;
	double hodlingAmount;
	double priceBuy;
	double priceSell;
	double priceCurrent;
	string storedIn;
	string mode;
};

struct prices {
	double buy;
	double sell;
	double current;
};


class BotMethod
{
	static void HodlingTradingHandler(_BotHodlingTradingModes mode);
	static void CheckHistoryHodlingTradingHandler(_BotHodlingTradingModes mode);
	static int _FillOldOrderParams(string StoredIn, string pair, double amount, OrderParams &orderParams);	
	static void _removeAnOrder(OrderParams &orderParams);
	static void _addLimitOrder(OrderParams &orderParams);
	static void _addStopLossOrder(OrderParams &orderParams);
	static bool _CompareOrderParams(OrderParams &old_order, OrderParams &new_order);
	static void _handleNewTrig(OrderParams &old_orderParams, OrderParams &new_orderParams);

public:
	static void CheckArbitrage();
	static void DollarPriceInRamzinex();
	static void Scalping();
	static void Hodling();
	static void Trading();
	static void HodlingTrading();
	static void Scalping2();

	static void CancelAllOrders(string &str, string exchange);
	static void SetJsonFileOrders();
	static void CheckHistoryHodling();
	static void CheckHistoryTrading();
	static void CheckHistoryHodlingTrading();

	static void ShowBalanceInUSDT();
	static void ShowAllExchangesPrice(int mode);
};

#endif