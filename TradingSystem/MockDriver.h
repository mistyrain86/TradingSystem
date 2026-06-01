#pragma once
#include <gmock/gmock.h>
#include "StockBroker.h"

class MockDriver : public StockBroker {
public:
    MOCK_METHOD(void, login, (const std::string& id, const std::string& pass), (override));
    MOCK_METHOD(void, buy, (const std::string& stockCode, const int price, const int count), (override));
    MOCK_METHOD(void, sell, (const std::string& stockCode, const int price, const int count), (override));
    MOCK_METHOD(int, getPrice, (const std::string& stockCode), (override));
};
