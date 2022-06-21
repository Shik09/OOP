#pragma once
#include <iostream>
using namespace std;
#include "Animal.h"

class Bird:public Animal
{
    public:
    Bird(int col)
    {
        color=col;
    }
    ~Bird()
    {
        if(color==0)
        cout<<"Red bird is gone."<<endl;
        else
        cout<<"Blue bird is gone."<<endl;
    }
   void  swim()
    {
        cout<<"Bird can not swim."<<endl;
    }
    void sing()
    {
         if(color==0)
        cout<<"Red bird is singing."<<endl;
        else
        cout<<"Blue bird is singing."<<endl;
    }
};