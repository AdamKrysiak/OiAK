#include "../include/Stoper.h"

#include <iostream>

using namespace std::chrono;


Stoper *Stoper::stoper = NULL;

Stoper::Stoper()
{
    this->duration=-1;
}
Stoper::~Stoper()
{

}



Stoper Stoper::getInstance()
{
    if(stoper==nullptr)
        stoper = new Stoper();
    return *stoper;

}

int Stoper::start()
{
    Stoper::t1 = high_resolution_clock::now();
}

int Stoper::stop()
{
    Stoper::t2= high_resolution_clock::now();
    duration = duration_cast<nanoseconds>( t2 - t1 ).count();
    return duration;
}

long Stoper::getLastMeasure()
{
    return duration;
}

void Stoper::printLastMeasure()
{
    std::cout<<"Czas: "<<duration<<std::endl;
}
