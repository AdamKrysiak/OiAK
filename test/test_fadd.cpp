#include <stdio.h>
#include <iostream>
#include <bitset>
#include "flib.h"
#include "gtest/gtest.h"


TEST(FlibTest, FaddTest){
	float a =10.123;
	float b =7.123;
	int score = fadd(a,b);
	EXPECT_EQ(*reinterpret_cast<float*>(&score),a+b);

}
TEST(FlipTest, FmulTest){
	float a =10.123;
	float b =7.123;

	int score = fmul(a,b);
		EXPECT_EQ(*reinterpret_cast<float*>(&score),a*b);
}
