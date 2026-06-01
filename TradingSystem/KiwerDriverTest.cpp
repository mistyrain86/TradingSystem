#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "KiwerDriver.h"

using namespace testing;

class KiwerDriverTest : public Test {
protected:
    KiwerDriver m_driver;

    template <typename Fn>
    std::string captureOutput(Fn fn) {
        testing::internal::CaptureStdout();
        fn();
        return testing::internal::GetCapturedStdout();
    }
};

TEST_F(KiwerDriverTest, Login_PrintsLoginSuccess) {
    auto output = captureOutput([&] { m_driver.login("kiwer_id", "pw123"); });
    EXPECT_THAT(output, HasSubstr("kiwer_id login success"));
}

TEST_F(KiwerDriverTest, Buy_PrintsBuyMessage) {
    // KiwerDriver: buy(stockCode, price, count) -> KiwerAPI.buy(stockCode, count, price)
    auto output = captureOutput([&] { m_driver.buy("005930", 70000, 10); });
    EXPECT_THAT(output, HasSubstr("005930 : Buy stock ( 70000 * 10)"));
}

TEST_F(KiwerDriverTest, Sell_PrintsSellMessage) {
    // KiwerDriver: sell(stockCode, price, count) -> KiwerAPI.sell(stockCode, count, price)
    auto output = captureOutput([&] { m_driver.sell("000270", 120000, 5); });
    EXPECT_THAT(output, HasSubstr("000270 : Sell stock ( 120000 * 5)"));
}

TEST_F(KiwerDriverTest, GetPrice_ReturnsValueInValidRange) {
    EXPECT_THAT(m_driver.getPrice("005930"), AllOf(Ge(5000), Le(5900)));
}
