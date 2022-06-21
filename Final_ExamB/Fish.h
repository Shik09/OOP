#pragma once
#include <iostream>
using namespace std;
#include "Animal.h"

class Fish:public Animal
{
    public:
    Fish(int col)
    {
        color=col;
    }

    ~Fish()
    {
        if(color==0)
        cout<<"Red fish is gone."<<endl;
        else
        cout<<"Blue fish is gone."<<endl;
    }
   void  sing()
    {
        cout<<"Fish can not sing."<<endl;
    }
    void swim()
    {
         if(color==0)
        cout<<"Red fish is swimming."<<endl;
        else
        cout<<"Blue fish is swimming."<<endl;
    }
};