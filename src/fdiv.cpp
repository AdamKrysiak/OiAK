#include <FPU.h>
#include <bitset>
#include <iostream>

int FPU::fdiv(float a, float b)
    {
    float temp = a / b;
    int float_score = *reinterpret_cast<int*>(&temp);

    // transfer float representation to an int
    unsigned int _a = *reinterpret_cast<int*>(&a);
    unsigned int _b = *reinterpret_cast<int*>(&b);

    // mantissa + hidden bit + GRS
    unsigned int man_a = ((_a & 0b00000000011111111111111111111111)
	    + 0b00000000100000000000000000000000);
    unsigned int man_b = ((_b & 0b00000000011111111111111111111111)
	    + 0b00000000100000000000000000000000);
    // exponent
    unsigned int exp_a = (_a & 0b01111111100000000000000000000000) >> 23;
    unsigned int exp_b = (_b & 0b01111111100000000000000000000000) >> 23;
    // sign
    unsigned int sign_a = _a & 0b10000000000000000000000000000000;
    unsigned int sign_b = _b & 0b10000000000000000000000000000000;
    // result
    unsigned int result = 0;

    if (exp_a == 0 || exp_b == 0)
  	throw FPU_Denormalized_Exception();
    //========================================================================
    //COUNTING
    //========================================================================

    // adding exponents
    unsigned int exp_c = exp_a - exp_b + 127;
    /*
     std::cout<<"exp_a:         "<<std::bitset<32>(exp_a)<<std::endl;
     std::cout<<"exp_b:         "<<std::bitset<32>(exp_b)<<std::endl;
     std::cout<<"exp_c:         "<<std::bitset<32>(exp_c)<<std::endl;
     */

    // signs xor
    unsigned int sign_c = sign_a ^ sign_b;

    // multiply mantissas
    //moving right for 20 not for 23 to have GRS bits
    unsigned long long man_c = (((long long) (man_a) << 23)
	    / ((long long) (man_b)) << 23) >> 20;

    /*

     std::cout<<"man_a:         "<<std::bitset<64>( (long long)(man_a) << 23 )<<std::endl;
     std::cout<<"man_b:         "<<std::bitset<64>( (long long)(man_b) << 23 )<<std::endl;
     std::cout<<"man_c:         "<<std::bitset<64>(man_c)<<std::endl;

     */

    //========================================================================
    //ROUNDING
    //========================================================================
    //rounding to plus infinitive
    int GRS_bits = man_c && 0b0000000000000000000000000000111;
    man_c = man_c >> 3;
    if (this->getRounding() == Rounding::PLUS_INF)		//+inf
	{

	if (GRS_bits >= 0x1) //001
	    {
	    if (sign_c == 0)		//if its negative (x<0), we just cut GRS
		man_c += 0b0000000000000000000000000000001;
	    }
	}
    else if (this->getRounding() == Rounding::NEAREST)		//to nearest
	{

	if (GRS_bits >= 0x3) //011
	    {
	    man_c += 0b0000000000000000000000000000001;
	    }
	}
    else if (this->getRounding() == Rounding::MINUS_INF) 		//-inf
	{

	if (GRS_bits >= 0x1) //001
	    {

	    if (sign_c != 0)
		man_c += 0b0000000000000000000000000000001;
	    }
	}
    //========================================================================
    //NORMALIZATION
    //========================================================================

    unsigned long long man_copy = man_c;
    int movement = 0;

    while (man_copy > 1)
	{
	man_copy = man_copy >> 1;
	movement++;
	}

    //counter -23 due to the difference of right number of bits and the number we got
    //normalize exponent
    movement = movement - 23;
    exp_c += movement;

    //check if exponent is ok
    if (exp_c > 255 || exp_c < 0)
	{
	//bad results, throw exception
	}

    //shift exponent to the right place
    exp_c = exp_c << 23;

    if (movement > 0)
	man_c = man_c >> (movement);
    else
	man_c = man_c << (-movement);

    //	std::cout<<"man_c:               "<<std::bitset<32>(man_c)<<std::endl;
    //subtract the hidden bit
    man_c = man_c ^ 0b00000000100000000000000000000000;

    /*
     std::cout<<"man_c:         "<<std::bitset<32>(man_c)<<std::endl;
     std::cout<<"exp_c:         "<<std::bitset<32>(exp_c)<<std::endl;
     */

    //========================================================================
    //EXCEPTIONS
    //========================================================================
    if (exp_c >= (255<<23))
	{
	if (man_c == 0)
	    {
	    if (sign_c == 1)
		throw FPU_minInf_Exception();
	    else
		throw FPU_plusInf_Exception();
	    }
	throw FPU_NAN_Exception();
	}
    else if (exp_c == 0)
	{
	if (man_c == 0)
	    {
	    if (sign_c == 1)
		throw FPU_minZero_Exception();
	    else
		throw FPU_plusZero_Exception();
	    }
	throw FPU_Denormalized_Exception();
	}
    //========================================================================
    //PUTTING THE RESULT TOGETHER
    //========================================================================
    // put the result together
    result = result | *reinterpret_cast<int*>(&man_c);
    result = result | sign_c;
    result = result | exp_c;

    float c = a / b;
    /*
     std::cout << "ourBinary:     " << std::bitset<32>(result) << std::endl;
     std::cout << "floatBinary:   "
     << std::bitset<32>(*reinterpret_cast<int*>(&c)) << std::endl;
     std::cout << "floatFPU:      " << a / b << std::endl;*/

    return result;
    }
