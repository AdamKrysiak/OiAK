#include "flib.h"
#include <iostream>
#include <bitset>
#include <iso646.h>

int fdiv(float a, float b)	{

	// transfer float representation to an int
	unsigned int _a = *reinterpret_cast<int*>(&a);
	unsigned int _b = *reinterpret_cast<int*>(&b);

	// mantissa + hidden bit + GRS
	unsigned int man_a = ((_a & 0b00000000011111111111111111111111) + 0b00000000100000000000000000000000);
	unsigned int man_b = ((_b & 0b00000000011111111111111111111111) + 0b00000000100000000000000000000000);
	// exponent
	unsigned int exp_a = (_a & 0b01111111100000000000000000000000) >> 23;
	unsigned int exp_b = (_b & 0b01111111100000000000000000000000) >> 23;
	// sign
	unsigned int sign_a = _a & 0b10000000000000000000000000000000;
	unsigned int sign_b = _b & 0b10000000000000000000000000000000;
	// result
	unsigned int result = 0;
	

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
	unsigned long long man_c = ( ( (long long)(man_a) << 23 ) / ( (long long)(man_b) ) << 23 ) >> 20;

/*

	std::cout<<"man_a:         "<<std::bitset<64>( (long long)(man_a) << 23 )<<std::endl;
	std::cout<<"man_b:         "<<std::bitset<64>( (long long)(man_b) << 23 )<<std::endl;
	std::cout<<"man_c:         "<<std::bitset<64>(man_c)<<std::endl;

*/


	//========================================================================
	//ROUNDING
	//========================================================================

	//rounding to plus infinitive
		int GRS_bits = man_c && 0b00000000000000000000000000000111;
		man_c = man_c>>3;
		if(GRS_bits>=0x3) //011
			if(sign_c==0)
				man_c+=0b00000000000000000000000000000001;
		else
			if(sign_c!=0)
				man_c+=0b00000000000000000000000000000001;


	//========================================================================
	//NORMALIZATION
	//========================================================================

	unsigned long long man_copy = man_c;
	int counter;

	while(man_copy > 1) {
			man_copy = man_copy >> 1;
			counter++;
		}

		//counter -23 due to the difference of right number of bits and the number we got
		//normalize exponent
		int movement = counter - 23;
		exp_c += movement;

		//check if exponent is ok
		if (exp_c > 255 || exp_c < 0) {
			//bad results, throw exception
		}

		//shift exponent to the right place
		exp_c = exp_c << 23;

		if(movement>0)
			man_c =  man_c>>(movement);
		else
			man_c =  man_c<<(-movement);

	//	std::cout<<"man_c:               "<<std::bitset<32>(man_c)<<std::endl;
		//subtract the hidden bit
		man_c = man_c ^ 0b00000000100000000000000000000000;


	/*
	std::cout<<"man_c:         "<<std::bitset<32>(man_c)<<std::endl;
	std::cout<<"exp_c:         "<<std::bitset<32>(exp_c)<<std::endl;
	*/

	//========================================================================
	//PUTTING THE RESULT TOGETHER
	//========================================================================

	// put the result together
	result = result | *reinterpret_cast<int*>(&man_c);
	result = result | sign_c;
	result = result | exp_c;
	

	float c = a / b;
	/*
	std::cout<<"ourBinary:     "<<std::bitset<32>(result)<<std::endl;
	std::cout<<"floatBinary:   "<<std::bitset<32>(*reinterpret_cast<int*>(&c))<<std::endl;
	std::cout<<"floatFPU:      "<<a/b<<std::endl;
	*/
	return result;
}
