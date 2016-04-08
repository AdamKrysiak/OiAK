#include "flib.h"
#include <iostream>
#include <bitset>
#include <iso646.h>

int fmul(float a, float b)	{
	// Znak(1) Exponenta(8) Mantysa(23)

	// zamiana float na uint
	unsigned int _a = *reinterpret_cast<int*>(&a);
	unsigned int _b = *reinterpret_cast<int*>(&b);

	// mantysa + poprzedzająca, ukryta 1
	unsigned int man_a = (_a & 0b00000000011111111111111111111111) + 0b00000000100000000000000000000000;
	unsigned int man_b = (_b & 0b00000000011111111111111111111111) + 0b00000000100000000000000000000000;
	// exponenta
	unsigned int exp_a = (_a & 0b01111111100000000000000000000000) >> 23;
	unsigned int exp_b = (_b & 0b01111111100000000000000000000000) >> 23;
	// znak
	unsigned int sign_a = _a & 0b10000000000000000000000000000000;
	unsigned int sign_b = _b & 0b10000000000000000000000000000000;
	
	// wynik
	unsigned int result = 0;
	
	// dodanie exponent
	unsigned int exp_c = exp_a + exp_b - 127;
	
	// xor znaków
	unsigned int sign_c = sign_a ^ sign_b;
	
	// mnożenie mantys z dodanym ukrytym bitem
	unsigned long long man_c = (long long)(man_a) * (long long)(man_b);
	// sprawdzanie czy wynik zostal zdenormalizowany
	// przesuniecie wyniku o 46 miejsc(domyślne miejsce przecinka po wymnożeniu)
	if((man_c>>46) > 1)	
	{
		// jesli tak
		unsigned long long man_copy = man_c>>46;
	
		// powiekszenie exponenty
		exp_c += 1;
		// sprawdzenie czy exponenta przekroczyła max
		if (exp_c > 255 || exp_c < 0)	{
			// zly wynik, poza granicami
		}
		// przywrocenie exponenty na odpowiednie miejsce
		exp_c = exp_c << 23;
		// przesuniecie wyniku w prawo do wyrównania
		// przecinek wyniku mnożenia jest 46 pozycji od prawej strony,
		// 23 z tych pozycji zajmuje mantysa, więc przesuwamy o 46-23 pozycji,
		// uwzględniamy przesuniecie o 1 przy normalizacji
		man_c = ( man_c >> (23 + 1) );
		// i odjęcie ukrytego bitu
		man_c = man_c ^ 0b00000000100000000000000000000000;
	}
	else	{
		exp_c = exp_c << 23;
//		std::cout<<"exp_c:         "<<std::bitset<32>(exp_c)<<std::endl;
		// jeśli nie, to po prostu przesuwamy liczbę w prawo do wyrównania o 46-23 pozycji
		man_c = man_c >> 23;
		// i ucinamy ukryty bit
		man_c = man_c ^ 0b00000000100000000000000000000000;
	}
	
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
	std::cout<<"floatFPU:      "<<a*b<<std::endl;
	return result;
}
