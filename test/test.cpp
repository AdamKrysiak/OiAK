#include <FPU.h>
#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>

float getRandomFloat()
{

	float randomValue = (((float) rand()) / (float) RAND_MAX) * (rand() % 10000)
			+ 1;
	return randomValue;
}
/*
 //----------------------------------TEST made to try single value
 TEST(FlibTest, FdivTest){
 float a =8528.36;
 float b =3101.92;
 int score = fdiv(a,b);
 std::cout<<*reinterpret_cast<float*>(&score)<<std::endl<<a/b;
 ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score),a/b);


 }*/

TEST(FlibTest, FaddTest)
{
	FPU fpu;
	std::srand(std::time(0));
	for (int i = 0; i < 10; i++)
	{
		float a = getRandomFloat();
		float b = getRandomFloat();

		int score = fpu.fadd(a, b);

		ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);
	}

}

TEST(FlibTest, FsubTest)
{
	FPU fpu;
	std::srand(std::time(0));
	for (int i = 0; i < 10; i++)
	{
		float a = getRandomFloat();
		float b = getRandomFloat();
		int score = fpu.fadd(a, -b);
		//std::cout<<a<<" "<<b<<std::endl;

		ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a - b);
	}

}

TEST(FlipTest, FmulTest)
{
	FPU fpu;
	std::srand(std::time(0));
	for (int i = 0; i < 10; i++)
	{
		float a = getRandomFloat();
		float b = getRandomFloat();
		int score = fpu.fmul(a, b);
		ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score),
				a * b);
	}
}

TEST(FlipTest, FdivTest)
{
	FPU fpu;
	std::srand(std::time(0));
	for (int i = 0; i < 100; i++)
	{
		float a = getRandomFloat();
		float b = getRandomFloat();
		int score = fpu.fdiv(a, b);
		std::cout << a << " " << b << std::endl;

		ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a / b);
	}
}

