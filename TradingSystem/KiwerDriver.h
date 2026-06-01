#pragma once
#include "StockBroker.h"
#include "kiwer_api.cpp"

class KiwerDriver : public StockBroker {
public:
    std::string getName() const override {
        return "KiwerDriver";
    }

    void login(const std::string& id, const std::string& pass) override {
        m_api.login(id, pass);
    }

    // KiwerAPI 인자 순서: buy(stockCode, count, price)
    void buy(const std::string& stockCode, int price, int count) override {
        m_api.buy(stockCode, count, price);
    }

    // KiwerAPI 인자 순서: sell(stockCode, count, price)
    void sell(const std::string& stockCode, int price, int count) override {
        m_api.sell(stockCode, count, price);
    }

    int getPrice(const std::string& stockCode) override {
        return m_api.currentPrice(stockCode);
    }

private:
    KiwerAPI m_api;
};
