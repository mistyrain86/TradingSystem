#pragma once
#include <gmock/gmock.h>
#include "ITimingStrategy.h"

class MockTimingStrategy : public ITimingStrategy {
public:
    MOCK_METHOD(bool, shouldBuy, (const std::vector<int>&), (override));
    MOCK_METHOD(bool, shouldSell, (const std::vector<int>&), (override));
};
