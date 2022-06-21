#include <iostream>
#include <string>
#include "Fraction.h"
using namespace std;

Fraction Fraction::operator-(Fraction f)
    {
        Fraction d;
        d.a=a*f.b-b*f.a;
        d.b=b*f.b;
        d.simplify();
        return d;
    }

ostream& operator<<(ostream& out,Fraction f)
{
    if(f.b==1)
    cout<<f.a;
    else cout<<f.a<<"/"<<f.b;
    return out;
}

int gcd(int a, int b){
    return b ? gcd(b, a % b) : a;
}