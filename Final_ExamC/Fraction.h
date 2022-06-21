#pragma once
#include <iostream>
#include <string>
using namespace std;

int gcd(int a, int b);
class Fraction
{
    public:
    long long a;
    long long b;
    void simplify()
    {
        if(a%b==0) {a=a/b;b=1; return;}
        long long m=a, n=b;
        m=gcd(m,n);
        a=a/m;b=b/m;
        if(b<0) {a=-a;b=-b;return;}
    }
    Fraction()
    {
        a=0;b=1;
    }
    Fraction(int x)
    {
        a=x;
        b=1;
    }
    Fraction& operator=(int x)
    {
        a=x;
        b=1;
        return *this;
    }
    Fraction operator/(Fraction f)
    {
        Fraction d;
        d.a=a*f.b;
        d.b=b*f.a;
        d.simplify();
        return d;
    }
    Fraction operator*(Fraction f)
    {
         Fraction d;
        d.a=a*f.a;
        d.b=b*f.b;
        d.simplify();
        return d;
    }
    Fraction operator+(Fraction f)
    {
        Fraction d;
        d.a=b*f.a+a*f.b;
        d.b=b*f.b;
        d.simplify();
        return d;
    }
     Fraction operator-(Fraction f);
     friend ostream & operator<<(ostream & out, Fraction A);
};

