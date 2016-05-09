#include <FPU.h>
#include <gtest/gtest.h>
#include <Stoper.h>
#include <cstdlib>
#include <ctime>

extern "C" void fpu_ctr(int x);

Stoper stoper = Stoper::getInstance();

float getRandomFloat()
    {

    float randomValue = (((float) rand()) / (float) RAND_MAX) * (rand() % 10000)
	    + 1;
    return randomValue;
    }
/*
TEST(FaddTest, Normal)
    {
    FPU fpu;
    fpu.setRounding(Rounding::NEAREST);
    fpu_ctr(0);
    float a = 10.124314;
    float b = 109418.124314;
    int score = fpu.fadd(a, b);

    stoper.start();			//przykład działania klasy Stoper
    fpu.fadd(a, b);
    stoper.stop();
    stoper.printLastMeasure();

    stoper.start();
    a + b;
    stoper.stop();
    stoper.printLastMeasure();

    ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);
    a = 11230.124314;
    b = 10948.2343144;
    score = fpu.fadd(a, b);
    ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);
    a = -11230.124314;
    b = 10.1231234;
    score = fpu.fadd(a, b);
    ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);

    a = 1278.13874;
    b = -81237.12441;
    score = fpu.fadd(a, b);
    ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);

    a = 1230.124314;
    b = -81210.321234;
    score = fpu.fadd(a, b);
    ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);

    a = 30.125324;
    b = 10.643234;
    score = fpu.fadd(a, b);
    ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);

    }

TEST(FaddTest, DenormalizedPassed)
    {
    FPU fpu;
    try
	{
	float a = 1.0E-40;		//denormalized
	float b = 0.124314;
	int score = fpu.fadd(a, b);
	ASSERT_EQ(0,1);
	}
    catch (FPU_Denormalized_Exception ex)
	{
	ASSERT_EQ(1, 1);
	}

    }
*/
TEST(FaddTest, DenormalizedResult)
    {
    FPU fpu;
    try
	{
	int intA = 0b00000001100100000000000000000000;
	int intB = 0b10000001100000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, b);
	ASSERT_EQ(0,1);
	}
    catch (FPU_Denormalized_Exception ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FaddTest, plusInf)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111111110000000000000000000000;
	int intB = 0b00111111100000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, b);
	ASSERT_EQ(0,1);
	}
    catch (FPU_plusInf_Exception ex)
	{
	ASSERT_EQ(1, 1);
	}
    }
TEST(FaddTest, minusInf)
    {
    FPU fpu;
    try
	{
	int intA = 0b11111111110000000000000000000000;
	int intB = 0b10111111100000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, b);
	ASSERT_EQ(0,1);
	}
    catch (FPU_minInf_Exception ex)
	{
	ASSERT_EQ(1, 1);
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
	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a * b);
	}
    }

TEST(FlipTest, FdivTest)
    {
    FPU fpu;
    for (int i = 0; i < 100; i++)
	{
	float a = 156.12356;
	float b = 6.12356;
	int score = fpu.fdiv(a, b);

	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a / b);
	}
    }

