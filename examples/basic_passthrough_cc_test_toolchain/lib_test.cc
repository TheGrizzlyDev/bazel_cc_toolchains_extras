#include <gtest/gtest.h>
#include "lib.hh"

TEST(LibTest, WritesFile) {
  EXPECT_TRUE(write_hello_world("test.txt"));
}
