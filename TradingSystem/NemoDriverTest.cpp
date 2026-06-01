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
    auto output = captureOutput([&] { m_driver.buy("005930", 70000, 10); });
    EXPECT_THAT(output, HasSubstr("[NEMO]005930 buy stock ( price : 70000 ) * ( count : 10)"));
}

TEST_F(NemoDriverTest, Sell_PrintsNemoSellMessage) {
    auto output = captureOutput([&] { m_driver.sell("000270", 120000, 5); });
    EXPECT_THAT(output, HasSubstr("[NEMO]000270 sell stock ( price : 120000 ) * ( count : 5)"));
}

TEST_F(NemoDriverTest, GetPrice_ReturnsValueInValidRange) {
    EXPECT_THAT(m_driver.getPrice("005930"), AllOf(Ge(5000), Le(5900)));
}
