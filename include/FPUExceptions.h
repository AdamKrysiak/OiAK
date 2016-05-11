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
    public:
    virtual const char* what() const throw ()
	{
	return "Not a number Exception occured";
	}
    };

class FPU_plusInf_Exception: public std::exception
    {
    public:
    virtual const char* what() const throw ()
	{
	return "Plus infinity Exception occured";
	}
    };

class FPU_minInf_Exception: public std::exception
    {
    public:
    virtual const char* what() const throw ()
	{
	return "Minus infinity Exception occured";
	}
    };
class FPU_Denormalized_Exception: public std::exception
    {
    public:
    virtual const char* what() const throw ()
	{
	return "Denormalized number Exception occured";
	}
    };

class FPU_plusZero_Exception: public std::exception
    {
    public:
    virtual const char* what() const throw ()
	{
	return "Plus zero number Exception occured";
	}
    };

class FPU_minZero_Exception: public std::exception
    {
    public:
    virtual const char* what() const throw ()
	{
	return "Minus zero number Exception occured";
	}
    };

class FPU_divideByZero_Exception: public std::exception
    {
    public:
    virtual const char* what() const throw ()
	{
	return "divide by zero Exception occured";
	}
    };
#endif /* SRC_FPUEXCEPTIONS_H_ */
