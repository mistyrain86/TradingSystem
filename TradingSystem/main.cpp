#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include "AutoTradingSystem.h"
#include "KiwerDriver.h"
#include "NemoDriver.h"
#include "MockDriver.h"

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();
    if (testResult != 0) return testResult;

    std::cout << "\n--- [Test 환경] Mock Driver 테스트 ---\n";
    AutoTradingSystem system;
    testing::NiceMock<MockDriver> mockBroker;
    ON_CALL(mockBroker, getPrice(testing::_)).WillByDefault(testing::Return(5500));
    system.selectStockBroker(&mockBroker);
    system.login("test_user", "password123");
    std::cout << "현재가 파악: " << system.getPrice("005930") << "\n\n";

    std::cout << "--- [실환경] 키워증권 연동 ---\n";
    KiwerDriver kiwerBroker;
    system.selectStockBroker(&kiwerBroker);
    system.login("kiwer_id", "kiwer_pass");
    system.buy("005930", 70000, 10);
    std::cout << "\n";

    std::cout << "--- [실환경] 네모증권 연동 ---\n";
    NemoDriver nemoBroker;
    system.selectStockBroker(&nemoBroker);
    system.login("nemo_id", "nemo_pass");
    system.sell("000270", 120000, 5);

    return 0;
}
