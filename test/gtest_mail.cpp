/*
 * gtest_mail.cpp
 *
 *  Created on: 08.04.2016
 *      Author: adam
 */

#include <stdio.h>
#include "gtest/gtest.h"

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


