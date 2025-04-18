#include "Map.hpp"
#include "unit_test_framework.hpp"


TEST(test_stub) {
    // Add your tests here
    Map<int, int> test; 
    test[1] = 5; 
    ASSERT_EQUAL(test[1], 5); 
    test[1] = 4;
    ASSERT_EQUAL(test[1], 4); 
    ASSERT_EQUAL(test.size(), 1); 
}

TEST_MAIN()
