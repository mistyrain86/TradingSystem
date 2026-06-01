#pragma once
#include <chrono>
#include <set>
#include <stdexcept>
#include <thread>
#include "StockBroker.h"

using std::string;

namespace {
    const string STOCKCODE_EXAMPLE = "005930";
    const string EMPTY_STOCKCODE = "";
    const string UNREGISTER_STOCKCODE = "99999999";
}

class AutoTradingSystem {
public:
    void selectStockBroker(StockBroker* broker) {
        m_broker = broker;
    }

    void registerStockCode(const string& stockCode) {
        m_registeredStockCodes.insert(stockCode);
    }

    void login(const string& id, const string& pass) {
        if (m_broker) m_broker->login(id, pass);
    }

    void buy(const string& stockCode, const int price, const int count) {
        checkInvalidStockCode(stockCode);
        if (m_broker) m_broker->buy(stockCode, price, count);
    }

    void sell(const string& stockCode, const int price, const int count) {
        checkInvalidStockCode(stockCode);
        if (m_broker) m_broker->sell(stockCode, price, count);
    }

    int getPrice(const string& stockCode) {
        checkInvalidStockCode(stockCode);
        if (m_broker) return m_broker->getPrice(stockCode);
        return 0;
    }

    // 200ms 주기로 3회 조회 후 상승 추세이면 totalAmount 한도 내 최대 수량 매수
    void buyNiceTiming(const string& stockCode, int totalAmount) {
        checkInvalidStockCode(stockCode);
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
    void sellNiceTiming(const string& stockCode, int count) {
        checkInvalidStockCode(stockCode);
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
    void checkInvalidStockCode(const string& stockCode)
    {
        if (stockCode.empty()) {
            throw std::invalid_argument("종목코드를 입력해주세요");
        }
        if (m_registeredStockCodes.find(stockCode) == m_registeredStockCodes.end()) {
            throw std::invalid_argument("등록되지 않은 종목코드입니다: " + stockCode);
        }
    }

    StockBroker* m_broker = nullptr;
    std::set<string> m_registeredStockCodes;
};
