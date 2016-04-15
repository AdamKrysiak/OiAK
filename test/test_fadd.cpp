#include <stdio.h>
#include <iostream>
#include <bitset>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "flib.h"
#include "gtest/gtest.h"


float getRandomFloat(){
	float randomValue = (((float)rand()) / (float) RAND_MAX)*(rand()%10000)+1;
	return randomValue;
}


//----------------------------------TEST made to try single value
/*TEST(FlibTest, FsubTest){
	float a =4112.2614;
	float b =5.5396;
	int score = fadd(a,b);
	std::cout<<*reinterpret_cast<float*>(&score)<<std::endl<<a+b;
	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score),a+b);


}*/



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
		//std::cout<<a<<" "<<b<<std::endl;

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
			int score = fdiv(a,b);
			std::cout<<a<<" "<<b<<std::endl;

			ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score),a/b);
}
}

