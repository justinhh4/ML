#include "TextBuffer.hpp"
#include "unit_test_framework.hpp"

using namespace std;

// Add your test cases here

TEST(test_up) {
  TextBuffer buffer;
  for (int i = 0; i < 5; i++) {
    buffer.insert((char)('0'+i));
  }
  buffer.insert('\n');
  for (int i = 0; i < 10; i++) {
    buffer.insert((char)('0'+i));
  }
  ASSERT_EQUAL(buffer.get_row(), 2);
  ASSERT_EQUAL(buffer.get_column(), 10);
  buffer.up();
  ASSERT_EQUAL(buffer.get_row(), 1);
  ASSERT_EQUAL(buffer.get_column(), 5);
}

TEST_MAIN()
