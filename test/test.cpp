#include <FPU.h>
#include <gtest/gtest.h>
#include <Stoper.h>
#include <cstdlib>
#include <ctime>



extern "C" void fpu_ctr(int x);
Stoper stoper = Stoper::getInstance();

float getRandomFloat()
{

	float randomValue = (((float) rand()) / (float) RAND_MAX) * (rand() % 10000)+ 1;
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

/*
TEST(fpu_ctr,roundingTest){
    float x = 23833/11;
    //fpu_ctr(0);
    std::cout<<"float_result:  "<<std::bitset<32>(*reinterpret_cast<int*>(&x))<<std::endl;
  //  fpu_ctr(1);
    std::cout<<"float_result:  "<<std::bitset<32>(*reinterpret_cast<int*>(&x))<<std::endl;
  //  fpu_ctr(2);
    std::cout<<"float_result:  "<<std::bitset<32>(*reinterpret_cast<int*>(&x))<<std::endl;


}*/

TEST(FaddTest, Normal)
{
	FPU fpu;
	fpu.setRounding(Rounding::NEAREST);
	fpu_ctr(0);
	float a = 10.124314;
	float b = 109418.124314;
	int score = fpu.fadd(a, b);

	stoper.start();				//przykład działania klasy Stoper
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


TEST(FaddTest, Small)
{
    FPU fpu;

    	float a = 0.441124314;
    	float b = 0.124314;
    	int score = fpu.fadd(a, b);
    	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);

    	a = 0.0124314;
    	b = 0.0343144;
    	score = fpu.fadd(a, b);
    	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);
    	a = 0.00124314;
    	b = 0.001231234;
    	score = fpu.fadd(a, b);
    	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);

    	a = 0.000124314;
    	b = 0.0001231234;
    	score = fpu.fadd(a, b);
    	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);

    	a = 0.00000124314;
    	b = -0.00000321234;
    	score = fpu.fadd(a, b);
    	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);

    	a = 0.000000125324;
    	b = 0.000000643234;
    	score = fpu.fadd(a, b);
    	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);


}


TEST(FaddTest, Big)
{
    FPU fpu;

    	float a = 441124.43;
    	float b = 124314.412;
    	int score = fpu.fadd(a, b);
    	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);

    	a = 124314.23463;
    	b = 343144.123;
    	score = fpu.fadd(a, b);
    	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);
    	a = 12431441;
    	b = 12312341;
    	score = fpu.fadd(a, b);
    	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);

    	a = 12434234534253514;
    	b = 12434234534253245;
    	score = fpu.fadd(a, b);
    	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);

    	a = 1243423453425324545;
    	b = 1243423453425324541;
    	score = fpu.fadd(a, b);
    	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);

    	a = 9223372036854775807;
    	b = 9223372036854775807;
    	score = fpu.fadd(a, b);
    	ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a + b);


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
	for (int i = 0; i < 100; i++)
	{
		float a = 156.12356;
		float b = 6.12356;
		int score = fpu.fdiv(a, b);

		ASSERT_FLOAT_EQ(*reinterpret_cast<float*>(&score), a / b);
	}
}

