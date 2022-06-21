#pragma once
#include <iostream>
#include <string>
#include "Fraction.h"
using namespace std;

template <typename T>
T calculate(char ex,int x,int y,int z)
{
    T a;
    if(ex=='A') 
    {
        a=(T)x*x+y*y+z*z;
        return a;
    }
    else if(ex=='B')
    {
        a=(T)x*(T)y+(T)y/(T)z-(T)x*(T)z;
        return a;
    }
    else
    {
        a=(T)y*3*(T)z-(T)x*(T)z/(T)(y-2)-(T)x*(T)y/(T)(z+1);
        return a;
    }
}