#include "lib.h"

#include <gtest/gtest.h>

#include <iostream>


TEST(Version_test, Empty) {

   // int version = version();
       
    ASSERT_GT(version(),0);
}

