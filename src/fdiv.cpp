#include <FPU.h>
#include <bitset>
#include <iostream>

int FPU::fdiv(float a, float b)
    {
    // transfer float representation to an int
    unsigned int _a = *reinterpret_cast<int*>(&a);
    unsigned int _b = *reinterpret_cast<int*>(&b);
    if (b == 0)
	throw FPU_divideByZero_Exception();

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
    int GRS_bits = 0;
    if (exp_a == 0 || exp_b == 0)
	throw FPU_Denormalized_Exception();

    //========================================================================
    //EXCEPTIONS IN PASSED
    //========================================================================

    if (exp_a == 0 || exp_b == 0)
	throw FPU_Denormalized_Exception();

    if (exp_a == 255)
	{
	if (man_a == (1 << 23))
	    {
	    if (sign_a == 0)
		throw FPU_plusInf_Exception();
	    else
		throw FPU_minInf_Exception();
	    }
	throw FPU_NAN_Exception();
	}
    if (exp_b == 255)
	{
	if (man_b == (1 << 23))
	    {
	    if (sign_b == 0)
		throw FPU_plusInf_Exception();
	    else
		throw FPU_minInf_Exception();
	    }
	throw FPU_NAN_Exception();
	}

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
	    / ((long long) (man_b)) << 23);
    int S_bit = man_c & 0b111111111111111111111;
    man_c = man_c >> 20;

    GRS_bits = man_c & 0b0000000000000000000000000000111;
    man_c = man_c >> 3;
    if (S_bit != 0)
	GRS_bits += 1;

    /*

     std::cout<<"man_a:         "<<std::bitset<64>( (long long)(man_a) << 23 )<<std::endl;
     std::cout<<"man_b:         "<<std::bitset<64>( (long long)(man_b) << 23 )<<std::endl;
     std::cout<<"man_c:         "<<std::bitset<64>(man_c)<<std::endl;

     */

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

    //counter -23 due to the difference of right number of bits and the number we got and GRS bits
    //normalize exponent
    movement = movement - 23;
    exp_c += movement;

    //========================================================================
    //EXCEPTIONS IN RESULT
    //========================================================================
    if ((int) exp_c >= 255)//this 3 is here becouse ealier we subtracted 26 not 23 from movement
	{

	if (sign_c == 0)
	    throw FPU_plusInf_Exception();
	else
	    throw FPU_minInf_Exception();

	}
    if ((int) exp_c <= 0)
	{
	/*if (man_c == 0)
	 {
	 if (sign_c == 1)
	 throw FPU_minZero_Exception();
	 else
	 throw FPU_plusZero_Exception();
	 }*/
	throw FPU_Denormalized_Exception();
	}
    //shift exponent to the right place

    ///CONTINUATION OF COUNTING
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
    //ROUNDING
    //========================================================================
    if (this->getRounding() == Rounding::PLUS_INF)		//+inf
	{

	if (sign_c == 0)		//if its negative (x<0), we just cut GRS
	    {
	    if (GRS_bits >= 0b10)
		man_c += 0b0000000000000000000000000000001;
	    }
	else			//if its negative (x<0), we just cut GRS
	    {
	    if (GRS_bits > 0b010)
		man_c += 0b0000000000000000000000000000001;
	    }
	}
    else if (this->getRounding() == Rounding::NEAREST)		//to nearest
	{

	if (GRS_bits > 0b010)
	    {
	    man_c += 0b0000000000000000000000000000001;
	    }
	if (GRS_bits == 0b010)
	    {
	    if (man_c & 0b1 != 0)
		man_c += 0b0000000000000000000000000000001;

	    }
	}
    else if (this->getRounding() == Rounding::MINUS_INF) 		//-inf
	{
	if (sign_c == 0)		//if its negative (x<0), we just cut GRS
	    {
	    if (GRS_bits > 0b10)
		man_c += 0b0000000000000000000000000000001;
	    }
	else			//if its negative (x<0), we just cut GRS
	    {
	    if (GRS_bits >= 0b10)
		man_c += 0b0000000000000000000000000000001;
	    }
	}
    //========================================================================
    //PUTTING THE RESULT TOGETHER
    //========================================================================
    // put the result together
    result = result | *reinterpret_cast<int*>(&man_c);
    result = result | sign_c;
    result = result | exp_c;

    /*

     std::cout << "ourBinary:     " << std::bitset<32>(result) << std::endl;
     std::cout << "floatBinary:   "
     << std::bitset<32>(*reinterpret_cast<int*>(&c)) << std::endl;
     std::cout << "floatFPU:      " << a / b << std::endl;

     */
    return result;
    }
