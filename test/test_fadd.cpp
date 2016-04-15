#include <stdio.h>
#include <iostream>
#include <bitset>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "flib.h"
#include "gtest/gtest.h"


float getRandomFloat(){
	float randomValue = (((float)rand()) / (float) RAND_MAX)*(rand()%100)+1;
	return randomValue;
}


TEST(FlibTest, FaddTest){
	std::srand(std::time(0));
	for(int i=0;i<10;i++){
		float a =getRandomFloat();
		float b =getRandomFloat();
		int score = fadd(a,b);
		ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score),a+b);
	}

}

TEST(FlibTest, FsubTest){
	std::srand(std::time(0));
	for(int i=0;i<10;i++){
		float a =getRandomFloat();
		float b =getRandomFloat();
		int score = fadd(a,-b);
		ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score),a-b);
	}

}

TEST(FlipTest, FmulTest){
	std::srand(std::time(0));
		for(int i=0;i<10;i++){
			float a =getRandomFloat();
			float b =getRandomFloat();
			int score = fmul(a,b);
			ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score),a*b);
}
}

TEST(FlipTest, FdivTest){
	std::srand(std::time(0));
		for(int i=0;i<10;i++){
			float a =getRandomFloat();
			float b =getRandomFloat();
			int score = fmul(a,b);
			ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score),a*b);
}
}

