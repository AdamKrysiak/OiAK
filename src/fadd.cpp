#include "flib.h"
#include <iostream>
#include <bitset>
#include <iso646.h>

int fadd(float a, float b)
{
	float float_score = a+b;
	//a musi być u mnie wieksze
	unsigned int _a;
	unsigned int _b;
	if(a<b)
	{
	    _a = *reinterpret_cast<int*>(&b);
	    _b = *reinterpret_cast<int*>(&a);
	}
	else
	{
	    _a = *reinterpret_cast<int*>(&a);
	    _b = *reinterpret_cast<int*>(&b);
	}

	unsigned int sign     = 0b10000000000000000000000000000000;
	unsigned int  mantisa = 0b00000000011111111111111111111111;
	unsigned int exponent = 0b01111111100000000000000000000000;


	//przesuwam jeszcze całość o 3 miejsca w prawo (dodaje 3 dodatkowe bity GRS)
	unsigned long man_a = ((_a & mantisa)+0b00000000100000000000000000000000)<<3;
	unsigned long man_b = (_b & mantisa)+0b00000000100000000000000000000000<<3;

	//przesuwam na poczatek i odejmuje obciążenie
	unsigned int exp_a = ((_a & exponent)>>23)-127;
	unsigned int exp_b = ((_b & exponent)>>23)-127;

/*

	std::cout<<"exp_a:              "<<std::bitset<32>(exp_a)<<std::endl;
	std::cout<<"exp_b:              "<<std::bitset<32>(exp_b)<<std::endl;
*/


	unsigned int sign_a = _a &sign;
	unsigned int sign_b = _b &sign;

	unsigned int sign_c;
	long long man_c;
	long long int exp_c;

	unsigned int difference_exp_ab;

	//rozpatruje sytuację w której mam te same znaki (dodawanie)
	if(sign_a==sign_b) {
		if(sign_a==0) {
		//obliczam różnice między wykładnikami
		difference_exp_ab = ((exp_a)-(exp_b));
/*		std::cout<<"difference_exp_ab:  "<<std::bitset<64>(difference_exp_ab)<<std::endl;*/

		//obliczam przesuniete a
		long long a_moved = man_a<<difference_exp_ab;
/*		std::cout<<"a_moved:            "<<std::bitset<64>(a_moved)<<std::endl;*/

		//obliczam zdenormalizowany wynik
		man_c = a_moved + man_b;
		/*std::cout<<"man_b:              "<<std::bitset<64>(man_b)<<std::endl;

		std::cout<<"man_c:              "<<std::bitset<64>(man_c)<<std::endl;
		std::cout<<"float_result:       "<<std::bitset<64>(*reinterpret_cast<int*>(&float_score))<<std::endl;*/

		//obliczam exponente c dodajac od razu obciazenie
		exp_c = (127+exp_b);
		}
		else{
			//obliczam różnice między wykładnikami
			difference_exp_ab = ((exp_b)-(exp_a));
			//obliczam przesuniete a
			long long b_moved = man_b<<difference_exp_ab;
			//obliczam zdenormalizowany wynik
			man_c = b_moved + man_a;
			//obliczam exponente c dodajac od razu obciazenie
			exp_c = (127+exp_a);
		}
	sign_c = sign_a;
	}
	//w tym przypadku działanie zmienia sie w odejmowanie
	else {
		if((man_a|exp_a)<=(man_b|exp_b)) {
			sign_c = sign_a;
			//obliczam różnice między wykładnikami
			difference_exp_ab = ((exp_a)-(exp_b));
			//obliczam przesuniete a
			long long a_moved = man_a<<difference_exp_ab;
			//obliczam zdenormalizowany wynik
			man_c = a_moved - man_b;
			//obliczam exponente c dodajac od razu obciazenie
			exp_c = (127+exp_b);
		}
		else {
			sign_c = sign_b;
			//obliczam różnice między wykładnikami
			difference_exp_ab = ((exp_b)-(exp_a));
			//obliczam przesuniete a
			long long b_moved = man_b<<difference_exp_ab;
			//obliczam zdenormalizowany wynik
			man_c = b_moved - man_a;
			//obliczam exponente c dodajac od razu obciazenie
			exp_c = (127+exp_a);
		}
	}

	//----------------------------------------------zaokrąglanie do +niekończoności
	int GRS_bits = man_c && 0b00000000000000000000000000000111;
	man_c = man_c>>3;
	if(GRS_bits>=0x3) //011
		if(sign==0)
			man_c+=0b00000000000000000000000000000001;
	else
		if(sign==1)
			man_c+=0b00000000000000000000000000000001;


/*
	std::cout<<"man_c:              "<<std::bitset<32>(man_c)<<std::endl;
	std::cout<<"exp_c:              "<<std::bitset<32>(exp_c)<<std::endl;
	std::cout<<"sign_c:             "<<std::bitset<32>(sign_c)<<std::endl;

	std::cout<<"float_result:       "<<std::bitset<32>(*reinterpret_cast<int*>(&float_score))<<std::endl;

	std::cout<<"----------------------------------------------------------------------------"<<std::endl;
*/
	//counter bedzie liczba miejsc po przecinku dla wyniku dodawania
	int counter = 0;
	unsigned long long man_copy = man_c;

	while(man_copy > 1) {
		man_copy = man_copy >> 1;
		counter++;
	}
/*	std::cout<<counter<<std::endl;*/

	//counter - 23, ponieważ jest to różnica między prawidłową liczbą bitów a tą którą uzyskałem
	// powiekszenie exponenty
	exp_c += counter-23;
	
	// sprawdzenie czy exponenta przekroczyła max
	if (exp_c > 255 || exp_c < 0) {
		// zly wynik, poza granicami
	}
	
	// przywrocenie exponenty na odpowiednie miejsce
	exp_c = exp_c << 23;


/*	std::cout<<"przesuniecie w prawo o:  "<<counter-23<<std::endl;*/

	man_c =  man_c>>(counter-23);
	// i odjęcie ukrytego bitu
	man_c = man_c ^ 0b00000000100000000000000000000000;
	
	/*std::cout<<"man_c:               "<<std::bitset<32>(man_c)<<std::endl;
	std::cout<<"exp_c:               "<<std::bitset<32>(exp_c)<<std::endl;
	std::cout<<"sign_c:              "<<std::bitset<32>(sign_c)<<std::endl;
	std::cout<<"float_result:        "<<std::bitset<32>(*reinterpret_cast<int*>(&float_score))<<std::endl;

	std::cout<<"----------------------------------------------------------------------------"<<std::endl;
*/
	unsigned int result = *reinterpret_cast<int*>(&man_c);
	result = result | sign_c;
	result = result | exp_c;

/*
	std::cout<<"difference     "<<std::bitset<32>(difference_exp_ab)<<std::endl;

	std::cout<<"_a:            "<<std::bitset<32>(_a)<<std::endl;
	std::cout<<"_b:            "<<std::bitset<32>(_b)<<std::endl;
	std::cout<<"man_a:         "<<std::bitset<32>(man_a)<<std::endl;
	std::cout<<"man_b:         "<<std::bitset<32>(man_b)<<std::endl;
	std::cout<<"man_c:         "<<std::bitset<32>(man_c)<<std::endl;

	std::cout<<"exp_a:         "<<std::bitset<32>(exp_a)<<std::endl;
	std::cout<<"exp_b:         "<<std::bitset<32>(exp_b)<<std::endl;
	std::cout<<"exp_c:         "<<std::bitset<32>(exp_c)<<std::endl;

	std::cout<<"result:        "<<std::bitset<32>(result)<<std::endl;
	std::cout<<"float_result:  "<<std::bitset<32>(*reinterpret_cast<int*>(&float_score))<<std::endl;


	std::cout<<"my_result:     "<<*reinterpret_cast<float*>(&result)<<std::endl;
	std::cout<<"float_result:  "<<a+b<<std::endl; */
	return result;
}
