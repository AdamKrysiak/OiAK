/*
 * FPU.cpp
 *
 *  Created on: 20.04.2016
 *      Author: adam
 */

#include "FPU.h"

FPU::FPU()
{
	this->rounding = Rounding::NEAREST;
}

void FPU::setRounding(Rounding round)
{
    this->rounding  = round;
}

Rounding FPU::getRounding()
{
	return this->rounding;
}

FPU::~FPU()
{
	// TODO Auto-generated destructor stub
}

