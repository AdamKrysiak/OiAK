/*
 * FPU.h
 *
 *  Created on: 20.04.2016
 *      Author: adam
 */

#ifndef FPU_H_
#define FPU_H_
#include "FPUExceptions.h"

enum class Rounding{ MINUS_INF = 0, NEAREST = 1, PLUS_INF = 2 };

class FPU
{


	Rounding rounding;

public:


	FPU();
	int fadd(float, float);
	int fmul(float, float);
	int fdiv(float, float);

	void setRounding(Rounding round);
	Rounding getRounding();


	virtual ~FPU();

};

#endif /* FPU_H_ */
