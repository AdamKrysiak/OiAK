#include <FPU.h>
#include <FPUExceptions.h>

int FPU::fadd(float a, float b)
    {
    float float_score = a + b;
    // a has to be bigger
    unsigned int _a;
    unsigned int _b;
    if (a < b)
	{
	_a = *reinterpret_cast<int*>(&b);
	_b = *reinterpret_cast<int*>(&a);
	}
    else
	{
	_a = *reinterpret_cast<int*>(&a);
	_b = *reinterpret_cast<int*>(&b);
	}

    unsigned int sign = 0b10000000000000000000000000000000;
    unsigned int mantissa = 0b00000000011111111111111111111111;
    unsigned int exponent = 0b01111111100000000000000000000000;

    //moving mantissa 3 bits left, now last 3 bits of score will be GRS
    unsigned long man_a = ((_a & mantissa) + 0b00000000100000000000000000000000)
	    << 3;
    unsigned long man_b = ((_b & mantissa) + 0b00000000100000000000000000000000)
	    << 3;

    // shift to the beginning and subtract excess
    unsigned int exp_a = ((_a & exponent) >> 23) - 127;
    unsigned int exp_b = ((_b & exponent) >> 23) - 127;

    /*

     std::cout<<"exp_a:              "<<std::bitset<32>(exp_a)<<std::endl;
     std::cout<<"exp_b:              "<<std::bitset<32>(exp_b)<<std::endl;
     std::cout<<"man_a:              "<<std::bitset<32>(man_a)<<std::endl;
     std::cout<<"man_b:              "<<std::bitset<32>(man_b)<<std::endl;
     */

    unsigned int sign_a = _a & sign;
    unsigned int sign_b = _b & sign;

    unsigned int sign_c;
    long long man_c;
    long long int exp_c;

    unsigned int difference_exp_ab;

    //========================================================================
    //EXCEPTIONS IN PASSED
    //========================================================================

    if (((_a & exponent) >> 23) == 0 || ((_b & exponent) >> 23) == 0)
	throw FPU_Denormalized_Exception();

    if (exp_a+127 == 255)		//127 is a balast
	{
	if (man_a == (1<<26))		//there is not zero because we add hided 1 and grs bits
	    {
	    if (sign_a == 0)
		throw FPU_plusInf_Exception();
	    else
		throw FPU_minInf_Exception();
	    }
	    throw FPU_NAN_Exception();
	}
    if (exp_b+127 == 255)
    	{
    	if (man_b == (1<<26))
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

    //addition with the same signs
    if (sign_a == sign_b)
	{
	if (sign_a == 0)
	    {
	    //difference between exponents
	    difference_exp_ab = ((exp_a) - (exp_b));
	    /*		std::cout<<"difference_exp_ab:  "<<std::bitset<64>(difference_exp_ab)<<std::endl;*/
	    //number of shifted bits
	    long long a_moved = man_a << difference_exp_ab;
	    /*		std::cout<<"a_moved:            "<<std::bitset<64>(a_moved)<<std::endl;*/
	    //denormalized result
	    man_c = a_moved + man_b;
	    /*std::cout<<"man_b:              "<<std::bitset<64>(man_b)<<std::endl;
	     std::cout<<"man_c:              "<<std::bitset<64>(man_c)<<std::endl;
	     std::cout<<"float_result:       "<<std::bitset<64>(*reinterpret_cast<int*>(&float_score))<<std::endl;*/
	    //result's exponent with excess
	    exp_c = (127 + exp_b);
	    }
	else
	    {
	    //difference between exponents
	    difference_exp_ab = ((exp_b) - (exp_a));
	    //number of shifted bits
	    long long b_moved = man_b << difference_exp_ab;
	    //denormalized result
	    man_c = b_moved + man_a;
	    //result's exponent with excess
	    exp_c = (127 + exp_a);
	    }
	sign_c = sign_a;
	}
    //if signs are different - subtract
    else
	{
	if ((exp_a) > (exp_b) || ((exp_a == exp_b) && (man_a > man_b)))
	    {
	    sign_c = sign_a;
	    //difference between exponents
	    difference_exp_ab = ((exp_a) - (exp_b));
	    //number of shifted bits
	    long long a_moved = man_a << difference_exp_ab;
	    //denormalized result
	    man_c = a_moved - man_b;
	    //result's exponent with excess
	    exp_c = (127 + exp_b);
	    }
	else
	    {
	    sign_c = sign_b;
	    //difference between exponents
	    difference_exp_ab = ((exp_b) - (exp_a));
	    //number of shifted bits
	    long long b_moved = man_b << difference_exp_ab;
	    //denormalized result
	    man_c = b_moved - man_a;
	    //result's exponent with excess
	    exp_c = (127 + exp_a);
	    }
	}

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
    /*
     std::cout<<"man_c:              "<<std::bitset<32>(man_c)<<std::endl;
     std::cout<<"exp_c:              "<<std::bitset<32>(exp_c)<<std::endl;
     std::cout<<"sign_c:             "<<std::bitset<32>(sign_c)<<std::endl;
     std::cout<<"float_result:       "<<std::bitset<32>(*reinterpret_cast<int*>(&float_score))<<std::endl;
     std::cout<<"----------------------------------------------------------------------------"<<std::endl;

     */
    //========================================================================
    //NORMALIZATION
    //========================================================================
    //counter is number of bits to shift when normalizing
    int counter = 0;
    unsigned long long man_copy = man_c;

    while (man_copy > 1)
	{
	man_copy = man_copy >> 1;
	counter++;
	}
    /*	std::cout<<counter<<std::endl;*/

    //counter -23 due to the difference of right number of bits and the number we got
    //normalize exponent
    int movement = counter - 23;
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

    //subtract the hidden bit
    man_c = man_c ^ 0b00000000100000000000000000000000;

    //========================================================================
    //EXCEPTIONS IN RESULT
    //========================================================================
    if (exp_c >= (255 << 23))
	{

	if (sign_c == 0)
	    throw FPU_plusInf_Exception();
	else
	    throw FPU_minInf_Exception();

	}
    else if (exp_c == 0)
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

    //========================================================================
    //PUTTING THE RESULT TOGETHER
    //========================================================================

    unsigned int result = *reinterpret_cast<int*>(&man_c);
    result = result | sign_c;
    result = result | exp_c;

    /*
     std::cout<<"difference:     "<<std::bitset<32>(difference_exp_ab)<<std::endl;

     std::cout<<"_a:            "<<std::bitset<32>(_a)<<std::endl;
     std::cout<<"_b:            "<<std::bitset<32>(_b)<<std::endl;
     std::cout<<"man_a:         "<<std::bitset<32>(man_a)<<std::endl;
     std::cout<<"man_b:         "<<std::bitset<32>(man_b)<<std::endl;
     std::cout<<"man_c:         "<<std::bitset<32>(man_c)<<std::endl;

     std::cout<<"exp_a:         "<<std::bitset<32>(exp_a)<<std::endl;
     std::cout<<"exp_b:         "<<std::bitset<32>(exp_b)<<std::endl;
     std::cout<<"exp_c:         "<<std::bitset<32>(exp_c)<<std::endl;


     */
    // std::cout<<"result:        "<<std::bitset<32>(result)<<std::endl;
    // std::cout<<"float_result:  "<<std::bitset<32>(*reinterpret_cast<int*>(&float_score))<<std::endl;
    return result;
    }
