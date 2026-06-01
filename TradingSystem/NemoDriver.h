#pragma once
#include "StockBroker.h"
#include "nemo_api.cpp"

class NemoDriver : public StockBroker {
public:
    std::string getName() const override {
        return "NemoDriver";
    }

    void login(const std::string& id, const std::string& pass) override {
        m_api.certification(id, pass);
    }

    void buy(const std::string& stockCode, const int price, const int count) override {
        m_api.purchasingStock(stockCode, price, count);
    }

    void sell(const std::string& stockCode, const int price, const int count) override {
        m_api.sellingStock(stockCode, price, count);
    }

    int getPrice(const std::string& stockCode) override {
        return m_api.getMarketPrice(stockCode, delayMinute);
    }

private:
    NemoAPI m_api;
    const int delayMinute = 1;
};
