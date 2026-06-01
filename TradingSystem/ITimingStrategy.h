#pragma once
#include <vector>

class ITimingStrategy {
public:
    virtual ~ITimingStrategy() = default;
    virtual bool shouldBuy(const std::vector<int>& priceHistory) = 0;
    virtual bool shouldSell(const std::vector<int>& priceHistory) = 0;
};
