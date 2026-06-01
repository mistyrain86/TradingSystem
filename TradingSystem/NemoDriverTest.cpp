#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "NemoDriver.h"

using namespace testing;

class NemoDriverTest : public Test {
protected:
    NemoDriver m_driver;

    template <typename Fn>
    std::string captureOutput(Fn fn) {
        testing::internal::CaptureStdout();
        fn();
        return testing::internal::GetCapturedStdout();
    }
};

TEST_F(NemoDriverTest, Login_PrintsNemoLoginGood) {
    auto output = captureOutput([&] { m_driver.login("nemo_id", "pw123"); });
    EXPECT_THAT(output, HasSubstr("[NEMO]nemo_id login GOOD"));
}

TEST_F(NemoDriverTest, Buy_PrintsNemoBuyMessage) {
    auto output = captureOutput([&] { m_driver.buy(STOCKCODE_EXAMPLE, STOCK_PRICE_EXAMPLE, STOCK_COUNT_EXAMPLE); });
    string logMessage = "[NEMO]" + STOCKCODE_EXAMPLE + " buy stock ( price : " + std::to_string(STOCK_PRICE_EXAMPLE) + " ) * ( count : " + std::to_string(STOCK_COUNT_EXAMPLE) + ")";
    EXPECT_THAT(output, HasSubstr(logMessage));
}

TEST_F(NemoDriverTest, Sell_PrintsNemoSellMessage) {
    auto output = captureOutput([&] { m_driver.sell(STOCKCODE_EXAMPLE, STOCK_PRICE_EXAMPLE, STOCK_COUNT_EXAMPLE); });
    string logMessage = "[NEMO]" + STOCKCODE_EXAMPLE + " sell stock ( price : " + std::to_string(STOCK_PRICE_EXAMPLE) + " ) * ( count : " + std::to_string(STOCK_COUNT_EXAMPLE) + ")";
    EXPECT_THAT(output, HasSubstr(logMessage));
}

TEST_F(NemoDriverTest, GetPrice_ReturnsValueInValidRange) {
    EXPECT_THAT(m_driver.getPrice(STOCKCODE_EXAMPLE), AllOf(Ge(5000), Le(5900)));
}
