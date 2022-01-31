//
// Created by justin on 10/23/21.
//

#include <gtest/gtest.h>
#include "RawFileFunctions.h"
namespace {
    TEST(raws, loadRawsRecursive) {
        loadRaws("../raws");
    }
}