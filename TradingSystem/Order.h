#pragma once
#include <string>

enum class OrderType { BUY, SELL };

struct Order {
    OrderType type;
    std::string stockCode;
    int price;
    int count;
};
