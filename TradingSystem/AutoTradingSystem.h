#pragma once
#include <chrono>
#include <iostream>
#include <thread>
#include "StockBroker.h"

class AutoTradingSystem {
public:
    void selectStockBroker(StockBroker* broker) {
        m_broker = broker;
        const std::string name = broker->getName();
        if (!name.empty()) {
            std::cout << "[selectStockBroker] " << name << "\n";
        }
    }

    void login(const std::string& id, const std::string& pass) {
        if (m_broker) m_broker->login(id, pass);
    }

    void buy(const std::string& stockCode, int price, int count) {
        if (m_broker) m_broker->buy(stockCode, price, count);
    }

    void sell(const std::string& stockCode, int price, int count) {
        if (m_broker) m_broker->sell(stockCode, price, count);
    }

    int getPrice(const std::string& stockCode) {
        if (m_broker) return m_broker->getPrice(stockCode);
        return 0;
    }

    // 200ms 주기로 3회 조회 후 상승 추세이면 totalAmount 한도 내 최대 수량 매수
    void buyNiceTiming(const std::string& stockCode, int totalAmount) {
        if (!m_broker) return;

        int prices[3];
        for (int i = 0; i < 3; ++i) {
            prices[i] = m_broker->getPrice(stockCode);
            if (i < 2) {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        }

        if (prices[0] < prices[1] && prices[1] < prices[2]) {
            const int count = totalAmount / prices[2];
            if (count > 0) {
                m_broker->buy(stockCode, prices[2], count);
            }
        }
    }

    // 200ms 주기로 3회 조회 후 하락 추세이면 count 수량 전량 매도
    void sellNiceTiming(const std::string& stockCode, int count) {
        if (!m_broker) return;

        int prices[3];
        for (int i = 0; i < 3; ++i) {
            prices[i] = m_broker->getPrice(stockCode);
            if (i < 2) {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        }

        if (prices[0] > prices[1] && prices[1] > prices[2]) {
            m_broker->sell(stockCode, prices[2], count);
        }
    }

private:
    StockBroker* m_broker = nullptr;
};
