#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "AutoTradingSystem.h"
#include "MockDriver.h"

using namespace testing;

class AutoTradingSystemTest : public Test {
protected:
    AutoTradingSystem m_system;
    MockDriver m_mockDriver;

    void SetUp() override {
        m_system.selectStockBroker(&m_mockDriver);
        m_system.registerStockCode(STOCKCODE_EXAMPLE);
    }
};

TEST_F(AutoTradingSystemTest, LoginDelegatesToBroker) {
    EXPECT_CALL(m_mockDriver, login("test_user", "test_pass"));
    m_system.login("test_user", "test_pass");
}

TEST_F(AutoTradingSystemTest, BuyDelegatesToBroker) {
    EXPECT_CALL(m_mockDriver, buy(STOCKCODE_EXAMPLE, 70000, 10));
    m_system.buy(STOCKCODE_EXAMPLE, 70000, 10);
}

TEST_F(AutoTradingSystemTest, SellDelegatesToBroker) {
    EXPECT_CALL(m_mockDriver, sell(STOCKCODE_EXAMPLE, 120000, 5));
    m_system.sell(STOCKCODE_EXAMPLE, 120000, 5);
}

TEST_F(AutoTradingSystemTest, GetPriceThrowsOnEmptyStockCode) {
    EXPECT_THROW(m_system.getPrice(EMPTY_STOCKCODE), std::invalid_argument);
}

TEST_F(AutoTradingSystemTest, GetPriceThrowsOnUnregisteredStockCode) {
    EXPECT_THROW(m_system.getPrice(UNREGISTER_STOCKCODE), std::invalid_argument);
}

TEST_F(AutoTradingSystemTest, GetPriceReturnsBrokerPrice) {
    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillOnce(Return(70000));
    EXPECT_EQ(70000, m_system.getPrice(STOCKCODE_EXAMPLE));
}

TEST_F(AutoTradingSystemTest, BuyNiceTimingBuysOnAscendingPrice) {
    // 5000 → 5100 → 5200 상승 추세: totalAmount(10000) / 5200 = 1주 매수
    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillOnce(Return(5000))
        .WillOnce(Return(5100))
        .WillOnce(Return(5200));
    EXPECT_CALL(m_mockDriver, buy(STOCKCODE_EXAMPLE, 5200, 1));

    m_system.buyNiceTiming(STOCKCODE_EXAMPLE, 10000);
}

TEST_F(AutoTradingSystemTest, BuyNiceTimingDoesNotBuyOnDescendingPrice) {
    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillOnce(Return(5200))
        .WillOnce(Return(5100))
        .WillOnce(Return(5000));
    EXPECT_CALL(m_mockDriver, buy(_, _, _))
        .Times(0);

    m_system.buyNiceTiming(STOCKCODE_EXAMPLE, 10000);
}

TEST_F(AutoTradingSystemTest, BuyNiceTimingDoesNotBuyOnFlatPrice) {
    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillOnce(Return(5000))
        .WillOnce(Return(5000))
        .WillOnce(Return(5000));
    EXPECT_CALL(m_mockDriver, buy(_, _, _))
        .Times(0);

    m_system.buyNiceTiming(STOCKCODE_EXAMPLE, 10000);
}

TEST_F(AutoTradingSystemTest, SellNiceTimingSellsOnDescendingPrice) {
    // 5200 → 5100 → 5000 하락 추세: 3주 전량 매도
    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillOnce(Return(5200))
        .WillOnce(Return(5100))
        .WillOnce(Return(5000));
    EXPECT_CALL(m_mockDriver, sell(STOCKCODE_EXAMPLE, 5000, 3));

    m_system.sellNiceTiming(STOCKCODE_EXAMPLE, 3);
}

TEST_F(AutoTradingSystemTest, SellNiceTimingDoesNotSellOnAscendingPrice) {
    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillOnce(Return(5000))
        .WillOnce(Return(5100))
        .WillOnce(Return(5200));
    EXPECT_CALL(m_mockDriver, sell(_, _, _))
        .Times(0);

    m_system.sellNiceTiming(STOCKCODE_EXAMPLE, 3);
}
