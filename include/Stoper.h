#ifndef STOPER_H
#define STOPER_H
#include <chrono>   ///tutaj jest klasa chrono::high_resolution_clock
#include <iostream>


class Stoper
{

public:
    static Stoper getInstance();
    int start();
    int stop();
    long getLastMeasure();
    void printLastMeasure();
    ~Stoper();

private:
    static Stoper *stoper;
    long duration;
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    Stoper();


};
#endif // STOPER_H
