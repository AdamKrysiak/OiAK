#include "flib.h"
#include <iostream>
#include <bitset>
#include <iso646.h>

int fdiv(float a, float b)	{

	// transfer float representation to an int
	unsigned int _a = *reinterpret_cast<int*>(&a);
	unsigned int _b = *reinterpret_cast<int*>(&b);

	// mantissa + hidden bit
	unsigned int man_a = (_a & 0b00000000011111111111111111111111) + 0b00000000100000000000000000000000;
	unsigned int man_b = (_b & 0b00000000011111111111111111111111) + 0b00000000100000000000000000000000;
	// exponent
	unsigned int exp_a = (_a & 0b01111111100000000000000000000000) >> 23;
	unsigned int exp_b = (_b & 0b01111111100000000000000000000000) >> 23;
	// sign
	unsigned int sign_a = _a & 0b10000000000000000000000000000000;
	unsigned int sign_b = _b & 0b10000000000000000000000000000000;
	
	// result
	unsigned int result = 0;
	
	// adding exponents
	unsigned int exp_c = exp_a - exp_b + 127;
	
	// signs xor
	unsigned int sign_c = sign_a ^ sign_b;
	
	// multiply mantissas
	unsigned long long man_c = ( ( (long long)(man_a) << 23 ) / ( (long long)(man_b) ) << 23 ) >> 23;
	std::cout<<"man_a:         "<<std::bitset<64>( (long long)(man_a) << 23 )<<std::endl;
	std::cout<<"man_b:         "<<std::bitset<64>( (long long)(man_b) << 23 )<<std::endl;
	std::cout<<"man_c:         "<<std::bitset<64>(man_c)<<std::endl;
	
	// subtract the hidden bit
	man_c = man_c ^ 0b00000000100000000000000000000000;
	std::cout<<"man_c:         "<<std::bitset<64>(man_c)<<std::endl;
	
	std::cout<<"exp_c:         "<<std::bitset<32>(exp_c)<<std::endl;
	exp_c = exp_c << 23;
	
	// put the result together
	result = result | *reinterpret_cast<int*>(&man_c);
	result = result | sign_c;
	result = result | exp_c;
	

	float c = a / b;
	std::cout<<"floatBinary:   "<<std::bitset<32>(*reinterpret_cast<int*>(&c))<<std::endl;
	std::cout<<"floatFPU:      "<<a/b<<std::endl;
	return result;
}
