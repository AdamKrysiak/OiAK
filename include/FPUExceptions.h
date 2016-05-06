/*
 * FPUExceptions.h
 *
 *  Created on: 06.05.2016
 *      Author: adam
 */

#ifndef SRC_FPUEXCEPTIONS_H_
#define SRC_FPUEXCEPTIONS_H_

#include <exception>

class FPU_NAN_Exception: public std::exception
    {
    virtual const char* what() const throw ()
	{
	return "Not a number Exception occured";
	}
    };

class FPU_plusInf_Exception: public std::exception
    {
    virtual const char* what() const throw ()
	{
	return "To big number Exception occured";
	}
    };

class FPU_minInf_Exception: public std::exception
    {
    virtual const char* what() const throw ()
	{
	return "To big number Exception occured";
	}
    };
class FPU_Denormalized_Exception: public std::exception
    {
    virtual const char* what() const throw ()
	{
	return "Denormalized number Exception occured";
	}
    };

#endif /* SRC_FPUEXCEPTIONS_H_ */