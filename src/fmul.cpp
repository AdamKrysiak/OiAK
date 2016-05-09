#include "FPU.h"
#include <iostream>
#include <bitset>
#include <iso646.h>

int FPU::fmul(float a, float b)
    {

    // transfer float representation to an int
    unsigned int _a = *reinterpret_cast<int*>(&a);
    unsigned int _b = *reinterpret_cast<int*>(&b);

    // mantissa + hidden bit
    unsigned int man_a = (_a & 0b00000000011111111111111111111111)
	    + 0b00000000100000000000000000000000;
    unsigned int man_b = (_b & 0b00000000011111111111111111111111)
	    + 0b00000000100000000000000000000000;
    // exponent
    unsigned int exp_a = (_a & 0b01111111100000000000000000000000) >> 23;
    unsigned int exp_b = (_b & 0b01111111100000000000000000000000) >> 23;

    if (exp_a == 0 || exp_b == 0)
	throw FPU_Denormalized_Exception();

    // sign
    unsigned int sign_a = _a & 0b10000000000000000000000000000000;
    unsigned int sign_b = _b & 0b10000000000000000000000000000000;

    // result
    unsigned int result = 0;

    // adding exponents
    unsigned int exp_c = exp_a + exp_b - 127;

    // signs xor
    unsigned int sign_c = sign_a ^ sign_b;

    // multiply mantissas
    unsigned long long man_c = (long long) (man_a) * (long long) (man_b);

    // check if result is denormalized
    // shift through 46 bits(place of the point)
    if ((man_c >> 46) > 1)
	{

	// add 1 to exponent
	exp_c += 1;
	// shift exponent to the right place
	exp_c = exp_c << 23;

	// shift result to the right
	// add +1 as we normalize
	// so it shoud be 23 + 1 but we have to know RS bits

	man_c = (man_c >> 22);

//========================================================================
//ROUNDING
//========================================================================

	//rounding to plus infinitive
	int RS_bits = man_c && 0b00000000000000000000000000000011;
	man_c = man_c >> 2;

	if (this->getRounding() == Rounding::PLUS_INF)		//+inf
	    {

	    if (RS_bits >= 0x1) //001
		{
		if (sign_c == 0)	//if its negative (x<0), we just cut RS
		    man_c += 0b0000000000000000000000000000001;
		}
	    }
	else if (this->getRounding() == Rounding::NEAREST)	//to nearest
	    {

	    if (RS_bits >= 0x3) //011
		{
		man_c += 0b0000000000000000000000000000001;
		}
	    }
	else if (this->getRounding() == Rounding::MINUS_INF) 		//-inf
	    {

	    if (RS_bits >= 0x1) //001
		{

		if (sign_c != 0)
		    man_c += 0b0000000000000000000000000000001;
		}
	    }

	// subtract the hidden bit
	man_c = man_c ^ 0b00000000100000000000000000000000;
	}
    else
	{
	//if it's normalized then just shift right
	man_c = man_c >> 23;
	// shift exponent to the right place
	exp_c = exp_c << 23;
	// subtract the hidden bit
	man_c = man_c ^ 0b00000000100000000000000000000000;
	}

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
    /*
     std::cout<<"mantysa        "<<std::bitset<32>(0b01111111100000000000000000000000)<<std::endl;
     std::cout<<"exponenta      "<<std::bitset<32>(0b00000000011111111111111111111111)<<std::endl;
     =======

     std::cout<<"mantissa       "<<std::bitset<32>(0b01111111100000000000000000000000)<<std::endl;
     std::cout<<"exponent       "<<std::bitset<32>(0b00000000011111111111111111111111)<<std::endl;
     >>>>>>> refs/remotes/origin/master
     std::cout<<"_a:            "<<std::bitset<32>(_a)<<std::endl;
     std::cout<<"_b:            "<<std::bitset<32>(_b)<<std::endl;
     std::cout<<"man_a:         "<<std::bitset<32>(man_a)<<std::endl;
     std::cout<<"man_b:         "<<std::bitset<32>(man_b)<<std::endl;
     std::cout<<"result:        "<<std::bitset<32>(result)<<std::endl;

     std::cout<<"resultFloat:   "<<*reinterpret_cast<float*>(&result)<<std::endl;
     std::cout<<"floatFPU:      "<<a*b<<std::endl;*/
    return result;
    }
