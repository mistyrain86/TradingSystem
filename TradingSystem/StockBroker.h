#pragma once
#include <string>

class StockBroker {
public:
    virtual ~StockBroker() = default;

    virtual std::string getName() const = 0;
    virtual void login(const std::string& id, const std::string& pass) = 0;
    virtual void buy(const std::string& stockCode, const int price, const int count) = 0;
    virtual void sell(const std::string& stockCode, const int price, const int count) = 0;
    virtual int getPrice(const std::string& stockCode) = 0;
};
