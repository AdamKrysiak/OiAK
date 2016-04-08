#include "flib.h"
#include <iostream>
#include <bitset>
#include <iso646.h>

int fdiv(float a, float b)	{
	// Znak(1) Exponenta(8) Mantysa(23)

	// zamiana float na uint
	unsigned int _a = *reinterpret_cast<int*>(&a);
	unsigned int _b = *reinterpret_cast<int*>(&b);

	// mantysa + poprzedzająca, ukryta 1
	unsigned int man_a = ((_a & 0b00000000011111111111111111111111) + 0b00000000100000000000000000000000);
	unsigned int man_b = ((_b & 0b00000000011111111111111111111111) + 0b00000000100000000000000000000000);
	// exponenta
	unsigned int exp_a = (_a & 0b01111111100000000000000000000000) >> 23;
	unsigned int exp_b = (_b & 0b01111111100000000000000000000000) >> 23;
	// znak
	unsigned int sign_a = _a & 0b10000000000000000000000000000000;
	unsigned int sign_b = _b & 0b10000000000000000000000000000000;
	
	// wynik
	unsigned int result = 0;
	
	// dodanie exponent
	unsigned int exp_c = exp_a - exp_b - 127;
	
	// xor znaków
	unsigned int sign_c = sign_a ^ sign_b;
	
	// dzielenie mantys z dodanym ukrytym bitem
	unsigned int man_c = 0;
	
	/*
	asm (
		"mov %[e], %[d]\n\t"
        : [man_c] "=rm" (man_c)
        : [man_a] "rm" (man_a), [man_b] "rm" (man_b)	
	);
	*/
	
	// sprawdzanie czy wynik zostal zdenormalizowany
	// przesuniecie wyniku o 46 miejsc(domyślne miejsce przecinka po wymnożeniu)
	std::cout<<"man_c:         "<<std::bitset<32>(man_c)<<std::endl;
	
	exp_c = exp_c << 23;
	std::cout<<"exp_c:         "<<std::bitset<32>(exp_c)<<std::endl;
	// jeśli nie, to po prostu przesuwamy liczbę w prawo do wyrównania o 46-23 pozycji
	man_c = man_c >> 23;
	// i ucinamy ukryty bit
	man_c = man_c ^ 0b00000000100000000000000000000000;

	// skladanie wyniku
	result = result | *reinterpret_cast<int*>(&man_c);
	result = result | sign_c;
	result = result | exp_c;
	
	std::cout<<"mantysa        "<<std::bitset<32>(0b01111111100000000000000000000000)<<std::endl;
	std::cout<<"exponenta      "<<std::bitset<32>(0b00000000011111111111111111111111)<<std::endl;
	std::cout<<"_a:            "<<std::bitset<32>(_a)<<std::endl;
	std::cout<<"_b:            "<<std::bitset<32>(_b)<<std::endl;
	std::cout<<"man_a:         "<<std::bitset<32>(man_a)<<std::endl;
	std::cout<<"man_b:         "<<std::bitset<32>(man_b)<<std::endl;
	std::cout<<"result:        "<<std::bitset<32>(result)<<std::endl;
	
	std::cout<<"resultFloat:   "<<*reinterpret_cast<float*>(&result)<<std::endl;
	std::cout<<"floatFPU:      "<<a/b<<std::endl;
	return result;
}
