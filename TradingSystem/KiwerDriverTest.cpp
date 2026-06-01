#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "KiwerDriver.h"

using namespace testing;

class KiwerDriverTest : public Test {
protected:
    KiwerDriver m_driver;

    template <typename Fn>
    std::string captureOutput(Fn fn) {
       internal::CaptureStdout();
        fn();
        return internal::GetCapturedStdout();
    }
};

TEST_F(KiwerDriverTest, Login_PrintsLoginSuccess) {
    auto output = captureOutput([&] { m_driver.login("kiwer_id", "pw123"); });
    EXPECT_THAT(output, HasSubstr("kiwer_id login success"));
}

TEST_F(KiwerDriverTest, Buy_PrintsBuyMessage) {
    auto output = captureOutput([&] { m_driver.buy(STOCKCODE_EXAMPLE, STOCK_PRICE_EXAMPLE, STOCK_COUNT_EXAMPLE); });
    string logMessage = STOCKCODE_EXAMPLE + " : Buy stock ( " + std::to_string(STOCK_PRICE_EXAMPLE) + " * " + std::to_string(STOCK_COUNT_EXAMPLE) + ")";
    EXPECT_THAT(output, HasSubstr(logMessage));
}

TEST_F(KiwerDriverTest, Sell_PrintsSellMessage) {
    auto output = captureOutput([&] { m_driver.sell(STOCKCODE_EXAMPLE, STOCK_PRICE_EXAMPLE, STOCK_COUNT_EXAMPLE); });
    string logMessage = STOCKCODE_EXAMPLE + " : Sell stock ( " + std::to_string(STOCK_PRICE_EXAMPLE) + " * " + std::to_string(STOCK_COUNT_EXAMPLE) + ")";
    EXPECT_THAT(output, HasSubstr(logMessage));
}

TEST_F(KiwerDriverTest, GetPrice_ReturnsValueInValidRange) {
    EXPECT_THAT(m_driver.getPrice(STOCKCODE_EXAMPLE), AllOf(Ge(5000), Le(5900)));
}
