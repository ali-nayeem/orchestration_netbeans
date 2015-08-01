/* 
 * File:   timer.h
 * Author: ali_nayeem
 *
 * Created on July 31, 2015, 5:54 PM
 */

#ifndef TIMER_H
#define	TIMER_H
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std::chrono;
class Timer
{
public:
    Timer()
    {
        start = high_resolution_clock::now();
        passedTime = 0;
    }
    virtual double operator()()
    {
        end =  high_resolution_clock::now();
        time_span = duration_cast<duration<double>>(end - start);
        passedTime = time_span.count();
        return passedTime;
    }
    const double & value()
    {
        return passedTime;
    }
private:
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    duration<double> time_span;
    double passedTime;
};


#endif	/* TIMER_H */

