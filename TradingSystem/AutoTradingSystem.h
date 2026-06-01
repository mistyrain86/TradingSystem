#pragma once
#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>
#include "Order.h"
#include "StockBroker.h"

using std::string;

namespace {
    const string STOCKCODE_EXAMPLE = "005930";
    const string EMPTY_STOCKCODE = "";
    const string UNREGISTER_STOCKCODE = "99999999";
}

class AutoTradingSystem {
public:
    AutoTradingSystem() : m_running(true) {
        m_workerThread = std::thread(&AutoTradingSystem::workerThread, this);
    }

    ~AutoTradingSystem() {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_running = false;
        }
        m_cv.notify_one();
        if (m_workerThread.joinable()) {
            m_workerThread.join();
        }
    }

    void selectStockBroker(StockBroker* broker) {
        m_broker = broker;
        const std::string name = broker->getName();
        if (!name.empty()) {
            std::cout << "[selectStockBroker] " << name << "\n";
        }
    }

    void scheduleOrder(const Order& order,
                       std::chrono::system_clock::time_point executeAt) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_orderQueue.push({order, executeAt});
        }
        m_cv.notify_one();
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
    struct ScheduledOrder {
        Order order;
        std::chrono::system_clock::time_point executeAt;

        bool operator>(const ScheduledOrder& other) const {
            return executeAt > other.executeAt;
        }
    };

    void workerThread() {
        while (true) {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_cv.wait(lock, [this] {
                return !m_running || !m_orderQueue.empty();
            });

            if (!m_running) break;

            const auto nextTime = m_orderQueue.top().executeAt;
            m_cv.wait_until(lock, nextTime);

            if (!m_running) break;

            const auto now = std::chrono::system_clock::now();
            while (!m_orderQueue.empty() &&
                   m_orderQueue.top().executeAt <= now) {
                ScheduledOrder scheduled = m_orderQueue.top();
                m_orderQueue.pop();
                lock.unlock();
                executeOrder(scheduled.order);
                lock.lock();
            }
        }
    }

    void executeOrder(const Order& order) {
        const auto now = std::chrono::system_clock::now();
        const auto timeT = std::chrono::system_clock::to_time_t(now);
        std::tm tm = {};
        localtime_s(&tm, &timeT);

        const string typeStr = (order.type == OrderType::BUY) ? "BUY" : "SELL";
        std::cout << "[scheduleOrder] "
                  << std::put_time(&tm, "%H:%M:%S") << " "
                  << typeStr << " "
                  << order.stockCode
                  << " price:" << order.price
                  << " count:" << order.count << "\n";

        try {
            if (order.type == OrderType::BUY) {
                buy(order.stockCode, order.price, order.count);
            } else {
                sell(order.stockCode, order.price, order.count);
            }
        } catch (const std::exception& e) {
            std::cout << "[scheduleOrder] error: " << e.what() << "\n";
        }
    }

    void checkInvalidStockCode(const string& stockCode) {
        if (stockCode.empty()) {
            throw std::invalid_argument("종목코드를 입력해주세요");
        }
        if (m_registeredStockCodes.find(stockCode) ==
            m_registeredStockCodes.end()) {
            throw std::invalid_argument("등록되지 않은 종목코드입니다: " +
                                        stockCode);
        }
    }

    StockBroker* m_broker = nullptr;
    std::set<string> m_registeredStockCodes;

    std::priority_queue<ScheduledOrder,
                        std::vector<ScheduledOrder>,
                        std::greater<ScheduledOrder>> m_orderQueue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_running = false;
    std::thread m_workerThread;
};
