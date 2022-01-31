//
// Created by Justin.Suess on 10/4/2021.
//
#include <gtest/gtest.h>
#include <iostream>
#include "testfunc.h"

namespace {

    TEST(example, add) {
        ASSERT_EQ(sum(3,4),7);
    }

}