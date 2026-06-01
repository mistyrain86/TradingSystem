#pragma once
#include <string>

using std::string;

namespace {
    const string STOCKCODE_EXAMPLE = "005930";
    const string EMPTY_STOCKCODE = "";
    const string UNREGISTER_STOCKCODE = "99999999";

    const int STOCK_PRICE_EXAMPLE = 70000;
    const int STOCK_COUNT_EXAMPLE = 5;

    const int TOTAL_AMOUNT = 10000;
}

class StockBroker {
public:
    virtual ~StockBroker() = default;

    virtual string getName() const = 0;
    virtual void login(const string& id, const string& pass) = 0;
    virtual void buy(const string& stockCode, const int price, const int count) = 0;
    virtual void sell(const string& stockCode, const int price, const int count) = 0;
    virtual int getPrice(const string& stockCode) = 0;
};
