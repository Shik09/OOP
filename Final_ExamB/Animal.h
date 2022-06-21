#pragma once
#include <iostream>
using namespace std;

class Animal
{
    public:
    int color;
    virtual ~Animal(){};
    virtual void swim()=0;
    virtual void sing()=0;
};