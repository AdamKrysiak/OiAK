#include <FPU.h>
#include <gtest/gtest.h>
#include <Stoper.h>
#include <cstdlib>
#include <fstream>
#include <ctime>

extern "C" void fpu_ctr(int x);

Stoper stoper = Stoper::getInstance();


TEST(FaddTest, Normal_RoundingMinusInf)
    {
    FPU fpu;
    fpu.setRounding(Rounding::MINUS_INF);
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

    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);
    a = 11230.124314;
    b = 10948.2343144;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);
    a = -11230.124314;
    b = 10.1231234;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);

    a = 1278.13874;
    b = -81237.12441;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);

    a = 1230.124314;
    b = -81210.321234;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);

    a = 30.125324;
    b = 10.643234;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);

    }
TEST(FaddTest, Normal_RoundingPlusInf)
    {
    FPU fpu;
    fpu.setRounding(Rounding::PLUS_INF);
    fpu_ctr(2);

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

    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);
    a = 11230.124314;
    b = 10948.2343144;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);
    a = -11230.124314;
    b = 10.1231234;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);

    a = 1278.13874;
    b = -81237.12441;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);

    a = 1230.124314;
    b = -81210.321234;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);

    a = 30.125324;
    b = 10.643234;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);

    }
TEST(FaddTest, Normal_RoundingNearest)
    {
    FPU fpu;
    fpu.setRounding(Rounding::NEAREST);
    fpu_ctr(1);

    float a = 10.5;
    float b = 109418.0;
    int score = fpu.fadd(a, b);

    stoper.start();			//przykład działania klasy Stoper
    fpu.fadd(a, b);
    stoper.stop();
    stoper.printLastMeasure();

    stoper.start();
    a + b;
    stoper.stop();
    stoper.printLastMeasure();

    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);
    a = 11230.124314;
    b = 10948.2343144;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);
    a = -11230.124314;
    b = 10.1231234;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);

    a = 1278.13874;
    b = -81237.12441;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);

    a = 1230.124314;
    b = -81210.321234;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);

    a = 30.125324;
    b = 10.643234;
    score = fpu.fadd(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a + b);

    }

TEST(FaddTest, DenormalizedPassed)
    {
    FPU fpu;
    try
	{
	float a = 1.0E-40;		//denormalized
	float b = 0.124314;
	int score = fpu.fadd(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_Denormalized_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}

    }

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
	ASSERT_EQ(0, 1);
	}
    catch (FPU_Denormalized_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FaddTest, plusInfResult)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111111010000000000000000000000;
	int intB = 0b01111111000000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_plusInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }
TEST(FaddTest, minusInfResult)
    {
    FPU fpu;
    try
	{
	int intA = 0b11111111010000000000000000000000;
	int intB = 0b11111111000000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_minInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FaddTest, minusInfPassed)
    {
    FPU fpu;
    try
	{
	int intA = 0b11111111100000000000000000000000;
	int intB = 0b11010111010111010100000010101110;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_minInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FaddTest, plusInfPassed)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111111100000000000000000000000;
	int intB = 0b01010111010111010100000010101110;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_plusInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }
TEST(FaddTest, NANPassed)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111111100011111110000000000000;		//NAN
	int intB = 0b01010111010111010100000010101110;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_NAN_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FsubTest, Normal)
    {
    FPU fpu;
    fpu.setRounding(Rounding::NEAREST);
    fpu_ctr(1);
    float a = 10.124314;
    float b = 109418.124314;
    int score = fpu.fadd(a, -b);

    ASSERT_EQ(*reinterpret_cast<float*>(&score), a - b);
    a = 11230.124314;
    b = 10948.2343144;
    score = fpu.fadd(a, -b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a - b);
    a = -11230.124314;
    b = 10.1231234;
    score = fpu.fadd(a, -b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a - b);

    a = 1278.13874;
    b = -81237.12441;
    score = fpu.fadd(a, -b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a - b);

    a = 1230.124314;
    b = -81210.321234;
    score = fpu.fadd(a, -b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a - b);

    a = 30.125324;
    b = 10.643234;
    score = fpu.fadd(a, -b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a - b);

    }

TEST(FsubTest, DenormalizedPassed)
    {
    FPU fpu;
    try
	{
	float a = 1.0E-40;		//denormalized
	float b = 0.124314;
	int score = fpu.fadd(a, -b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_Denormalized_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}

    }

TEST(FsubTest, DenormalizedResult)
    {
    FPU fpu;
    try
	{
	int intA = 0b00000001100100000000000000000000;
	int intB = 0b00000001100000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, -b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_Denormalized_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FsubTest, plusInfResult)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111111010000000000000000000000;
	int intB = 0b11111111000000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, -b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_plusInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }
TEST(FsubTest, minusInfResult)
    {
    FPU fpu;
    try
	{
	int intA = 0b11111111010000000000000000000000;
	int intB = 0b01111111000000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, -b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_minInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FsubTest, minusInfPassed)
    {
    FPU fpu;
    try
	{
	int intA = 0b11111111100000000000000000000000;
	int intB = 0b11010111010111010100000010101110;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, -b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_minInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FsubTest, plusInfPassed)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111111100000000000000000000000;
	int intB = 0b01010111010111010100000010101110;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, -b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_plusInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }
TEST(FsubTest, NANPassed)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111111100011111110000000000000;		//NAN
	int intB = 0b01010111010111010100000010101110;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fadd(a, -b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_NAN_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FmulTest, Normal_RoundingPlusInf)
    {
    FPU fpu;
    fpu.setRounding(Rounding::PLUS_INF);
    fpu_ctr(2);
    float a = 10.124314;
    float b = 109418.124314;
    int score = fpu.fmul(a, b);

    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);
    a = 11230.124314;
    b = 10948.2343144;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);
    a = -11230.124314;
    b = 10.1231234;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);

    a = 1278.13874;
    b = -81237.12441;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);

    a = 1230.124314;
    b = -81210.321234;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);

    a = 30.125324;
    b = 10.643234;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);

    }

TEST(FmulTest, Normal_RoundingMinusInf)
    {
    FPU fpu;
    fpu.setRounding(Rounding::MINUS_INF);
    fpu_ctr(0);
    float a = 10.124314;
    float b = 109418.124314;
    int score = fpu.fmul(a, b);

    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);
    a = 11230.124314;
    b = 10948.2343144;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);
    a = -11230.124314;
    b = 10.1231234;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);

    a = 1278.13874;
    b = -81237.12441;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);

    a = 1230.124314;
    b = -81210.321234;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);

    a = 30.125324;
    b = 10.643234;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);

    }

TEST(FmulTest, Normal_RoundingNearest)
    {
    FPU fpu;
    fpu.setRounding(Rounding::NEAREST);
    fpu_ctr(1);
    float a = 10.124314;
    float b = 109418.124314;
    int score = fpu.fmul(a, b);

    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);
    a = 11230.124314;
    b = 10948.2343144;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);
    a = -11230.124314;
    b = 10.1231234;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);

    a = 1278.13874;
    b = -81237.12441;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);

    a = 1230.124314;
    b = -81210.321234;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);

    a = 30.125324;
    b = 10.643234;
    score = fpu.fmul(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a * b);

    }

TEST(FmulTest, DenormalizedPassed)
    {
    FPU fpu;
    try
	{
	float a = 1.0E-40;		//denormalized
	float b = 0.124314;
	int score = fpu.fmul(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_Denormalized_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}

    }

TEST(FmulTest, DenormalizedResult)
    {
    FPU fpu;
    try
	{
	int intA = 0b00000000100100000000000000000000;
	int intB = 0b00000000100000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fmul(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_Denormalized_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FmulTest, plusInfResult)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111000010000000000000000000000;
	int intB = 0b01110111000000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fmul(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_plusInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }
TEST(FmulTest, minusInfResult)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111000010000000000000000000000;
	int intB = 0b11110111000000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fmul(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_minInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FmulTest, minusInfPassed)
    {
    FPU fpu;
    try
	{
	int intA = 0b11111111100000000000000000000000;
	int intB = 0b11010111010111010100000010101110;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fmul(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_minInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FmulTest, plusInfPassed)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111111100000000000000000000000;
	int intB = 0b01010111010111010100000010101110;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fmul(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_plusInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }
TEST(FmulTest, NANPassed)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111111100011111110000000000000;		//NAN
	int intB = 0b01010111010111010100000010101110;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fmul(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_NAN_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FdivTest, Normal_RoundingMinusInf)
    {
    FPU fpu;
    fpu.setRounding(Rounding::MINUS_INF);
    fpu_ctr(0);
    float a = 10.124314;
    float b = 109418.124314;
    int score = fpu.fdiv(a, b);

    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);
    a = 11230.287144;
    b = 10948.287144;
    score = fpu.fdiv(a, b);

    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);
    a = -11230, 4231234;
    b = 10000.4231234;

    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    a = 1278.13874;
    b = -81237.12441;
    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    a = 12030.124314;
    b = -81210.321234;
    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    a = 30.125324;
    b = 10.643234;
    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    }

TEST(FdivTest, Normal_RoundingPlusInf)
    {
    FPU fpu;
    fpu.setRounding(Rounding::PLUS_INF);
    fpu_ctr(2);
    float a = 10.124314;
    float b = 109418.124314;
    int score = fpu.fdiv(a, b);

    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);
    a = 11230.287144;
    b = 10948.287144;
    score = fpu.fdiv(a, b);

    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);
    a = -11230, 4231234;
    b = 10000.4231234;

    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    a = 1278.13874;
    b = -81237.12441;
    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    a = 12030.124314;
    b = -81210.321234;
    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    a = 30.125324;
    b = 10.643234;
    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    }

TEST(FdivTest, Normal_RoundingNearest)
    {
    FPU fpu;
    fpu.setRounding(Rounding::NEAREST);
    fpu_ctr(1);

    float a = 10.124314;
    float b = 109418.124314;
    int score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    a = 1278.13874;
    b = -81237.12441;
    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    a = 12030.124314;
    b = -81210.321234;
    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    a = 30.125324;
    b = 10.643234;
    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    a = 11230.287144;
    b = 10948.287144;
    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    a = -11230, 4231234;
    b = 10000.4231234;
    score = fpu.fdiv(a, b);
    ASSERT_EQ(*reinterpret_cast<float*>(&score), a / b);

    }

TEST(FdivTest, DenormalizedPassed)
    {
    FPU fpu;
    try
	{
	float a = 1.0E-40;		//denormalized
	float b = 0.124314;
	int score = fpu.fdiv(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_Denormalized_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}

    }

TEST(FdivTest, DenormalizedResult)
    {
    FPU fpu;
    try
	{
	int intA = 0b00000001100100000000000000000000;
	int intB = 0b01111100100000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fdiv(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_Denormalized_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FdivTest, plusInfResult)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111101010000000000000000000000;
	int intB = 0b00000010000000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fdiv(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_plusInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }
TEST(FdivTest, minusInfResult)
    {
    FPU fpu;
    try
	{
	int intA = 0b11111101010000000000000000000000;
	int intB = 0b00000010000000000000000000000000;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fdiv(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_minInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FdivTest, minusInfPassed)
    {
    FPU fpu;
    try
	{
	int intA = 0b11111111100000000000000000000000;
	int intB = 0b11010111010111010100000010101110;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fdiv(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_minInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FdivTest, plusInfPassed)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111111100000000000000000000000;
	int intB = 0b01010111010111010100000010101110;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fdiv(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_plusInf_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }
TEST(FdivTest, NANPassed)
    {
    FPU fpu;
    try
	{
	int intA = 0b01111111100011111110000000000000;		//NAN
	int intB = 0b01010111010111010100000010101110;
	float a = *reinterpret_cast<float*>(&intA);		//denormalized
	float b = *reinterpret_cast<float*>(&intB);
	int score = fpu.fdiv(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_NAN_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }

TEST(FdivTest, ZeroPassed)
    {
    FPU fpu;
    try
	{
	float a= 5616116;
	float b = 0;
	int score = fpu.fdiv(a, b);
	ASSERT_EQ(0, 1);
	}
    catch (FPU_divideByZero_Exception &ex)
	{
	ASSERT_EQ(1, 1);
	}
    }
/*
TEST(SpeedTest, speedTest)
    {

    FPU fpu;
    fpu_ctr(1);
    std::fstream file("wyniki.txt", std::ios::out);
    float a = 56115.156;
    float b = 15616.65165;

    file << "NASZ FPU DODAWANIE\n";
    for (int i = 0; i < 50; ++i)
	{
	stoper.start();
	fpu.fadd(a, b);
	stoper.stop();
	file << stoper.getLastMeasure() << std::endl;
	}
    file << "ORYGINALNY FPU DODAWANIE\n";
    for (int i = 0; i < 50; ++i)
	{
	stoper.start();
	a + b;
	stoper.stop();
	file << stoper.getLastMeasure() << std::endl;
	}
    file << "NASZ FPU MNOŻENIE\n";
    for (int i = 0; i < 50; ++i)
	{
	stoper.start();
	fpu.fmul(a, b);
	stoper.stop();
	file << stoper.getLastMeasure() << std::endl;
	}
    file << "ORYGINALNY FPU MNOŻENIE\n";
    for (int i = 0; i < 50; ++i)
	{
	stoper.start();
	a * b;
	stoper.stop();
	file << stoper.getLastMeasure() << std::endl;
	}
    file << "NASZ FPU Dzielenie\n";
    for (int i = 0; i < 50; ++i)
	{
	stoper.start();
	fpu.fdiv(a, b);
	stoper.stop();
	file << stoper.getLastMeasure() << std::endl;
	}
    file << "ORYGINALNY FPU Dzielenie\n";
    for (int i = 0; i < 50; ++i)
	{
	stoper.start();
	a / b;
	stoper.stop();
	file << stoper.getLastMeasure() << std::endl;
	}

    file.close();

    }
*/

