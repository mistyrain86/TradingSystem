#pragma once
#include "StockBroker.h"
#include "kiwer_api.cpp"

class KiwerDriver : public StockBroker {
public:
    void login(const std::string& id, const std::string& pass) override {
        m_api.login(id, pass);
    }

    void buy(const std::string& stockCode, const int price, const int count) override {
        m_api.buy(stockCode, count, price);
    }

    void sell(const std::string& stockCode, const int price, const int count) override {
        m_api.sell(stockCode, count, price);
    }

    int getPrice(const std::string& stockCode) override {
        return m_api.currentPrice(stockCode);
    }

private:
    KiwerAPI m_api;
};
