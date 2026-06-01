#pragma once
#include <string>

class StockBroker {
public:
    virtual ~StockBroker() = default;

    virtual void login(const std::string& id, const std::string& pass) = 0;
    virtual void buy(const std::string& stockCode, int price, int count) = 0;
    virtual void sell(const std::string& stockCode, int price, int count) = 0;
    virtual int getPrice(const std::string& stockCode) = 0;
};
