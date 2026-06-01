#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "AutoTradingSystem.h"
#include "MockDriver.h"
#include "MockTimingStrategy.h"

using namespace testing;

class TimingStrategyTest : public Test {
protected:
    AutoTradingSystem m_system;
    NiceMock<MockDriver> m_mockDriver;
    MockTimingStrategy m_mockStrategy;

    void SetUp() override {
        ON_CALL(m_mockDriver, getName())
            .WillByDefault(Return(""));
        m_system.selectStockBroker(&m_mockDriver);
        m_system.registerStockCode(STOCKCODE_EXAMPLE);
        m_system.setTimingStrategy(&m_mockStrategy);
    }
};

TEST_F(TimingStrategyTest, BuyNiceTiming_CallsStrategyWithCollectedPrices) {
    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillOnce(Return(5000))
        .WillOnce(Return(5100))
        .WillOnce(Return(5200));
    EXPECT_CALL(m_mockStrategy, shouldBuy(ElementsAre(5000, 5100, 5200)))
        .WillOnce(Return(false));

    m_system.buyNiceTiming(STOCKCODE_EXAMPLE, TOTAL_AMOUNT);
}

TEST_F(TimingStrategyTest, BuyNiceTiming_BuysWhenStrategyReturnsTrue) {
    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillOnce(Return(5000))
        .WillOnce(Return(5100))
        .WillOnce(Return(5200));
    EXPECT_CALL(m_mockStrategy, shouldBuy(_))
        .WillOnce(Return(true));
    EXPECT_CALL(m_mockDriver, buy(STOCKCODE_EXAMPLE, 5200, 1));

    m_system.buyNiceTiming(STOCKCODE_EXAMPLE, TOTAL_AMOUNT);
}

TEST_F(TimingStrategyTest, BuyNiceTiming_DoesNotBuyWhenStrategyReturnsFalse) {
    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillRepeatedly(Return(5000));
    EXPECT_CALL(m_mockStrategy, shouldBuy(_))
        .WillOnce(Return(false));
    EXPECT_CALL(m_mockDriver, buy(_, _, _))
        .Times(0);

    m_system.buyNiceTiming(STOCKCODE_EXAMPLE, TOTAL_AMOUNT);
}

TEST_F(TimingStrategyTest, SellNiceTiming_CallsStrategyWithCollectedPrices) {
    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillOnce(Return(5200))
        .WillOnce(Return(5100))
        .WillOnce(Return(5000));
    EXPECT_CALL(m_mockStrategy, shouldSell(ElementsAre(5200, 5100, 5000)))
        .WillOnce(Return(false));

    m_system.sellNiceTiming(STOCKCODE_EXAMPLE, 3);
}

TEST_F(TimingStrategyTest, SellNiceTiming_SellsWhenStrategyReturnsTrue) {
    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillOnce(Return(5200))
        .WillOnce(Return(5100))
        .WillOnce(Return(5000));
    EXPECT_CALL(m_mockStrategy, shouldSell(_))
        .WillOnce(Return(true));
    EXPECT_CALL(m_mockDriver, sell(STOCKCODE_EXAMPLE, 5000, 3));

    m_system.sellNiceTiming(STOCKCODE_EXAMPLE, 3);
}

TEST_F(TimingStrategyTest, SellNiceTiming_DoesNotSellWhenStrategyReturnsFalse) {
    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillRepeatedly(Return(5000));
    EXPECT_CALL(m_mockStrategy, shouldSell(_))
        .WillOnce(Return(false));
    EXPECT_CALL(m_mockDriver, sell(_, _, _))
        .Times(0);

    m_system.sellNiceTiming(STOCKCODE_EXAMPLE, 3);
}

TEST_F(TimingStrategyTest, SetTimingStrategy_ReplacedStrategyIsUsed) {
    MockTimingStrategy secondStrategy;

    EXPECT_CALL(m_mockDriver, getPrice(STOCKCODE_EXAMPLE))
        .WillRepeatedly(Return(5000));

    EXPECT_CALL(m_mockStrategy, shouldBuy(_))
        .WillOnce(Return(false));
    m_system.buyNiceTiming(STOCKCODE_EXAMPLE, TOTAL_AMOUNT);

    m_system.setTimingStrategy(&secondStrategy);

    EXPECT_CALL(secondStrategy, shouldBuy(_))
        .WillOnce(Return(false));
    m_system.buyNiceTiming(STOCKCODE_EXAMPLE, TOTAL_AMOUNT);
}
