#pragma once
#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
using namespace std;

template<class T>
class Data
{
public:
    shared_ptr<Data<T>> point_;
    int length;
public:
    Data(int _length) : length(_length){}
    virtual const T& get(int index)=0;
};

template<class T>
class DataEmpty : public Data<T>
{
public:
    DataEmpty<T>(): Data<T>(0) {};
    const T& get(int index) {
        assert(0); // No data in DataEmpty
    }
};

template<class T>
class DataPushBackDecorator : public Data<T>
{
    public:
    int _index;T end;
    // Implement a Decorator for push_back operation
    DataPushBackDecorator(shared_ptr<Data<T>> data,const T &value):Data<T>(data->length)
    {
        this->point_=data;
        _index=data->length;
        this->length=data->length+1;
        end=value;
    }
    const T& get(int index)
    {
        if(_index==index) return end;
        else{return (*(this->point_)).get(index);}
    }
};

template<class T>
class DataSetDecorator : public Data<T>
{
    public:
    int _index;T _value;
    // Implement a Decorator for set operation
    DataSetDecorator(shared_ptr<Data<T>> data,int index,const T &value):Data<T>(data->length)
    {
        this->point_=data;
        _index=index;
        _value=value;
    }
    const T& get(int index)
    {
        if(_index==index) return _value;
        else{return (*(this->point_)).get(index);}
    }
};


template<class T>
class PVector
{
public:
    shared_ptr<Data<T>> data;
    PVector<T>() {
        data = make_shared<DataEmpty<T>>();
        // when to delete??
    }
    PVector<T>(shared_ptr<Data<T>> _data): data(_data) { }
    PVector<T> push_back(const T &value) const{
        return PVector<T>(make_shared<DataPushBackDecorator<T>>(data, value));
    }
    PVector<T> set(int index, const T &value) const{
        return PVector<T>(make_shared<DataSetDecorator<T>>(data, index, value));
    }

    const T& operator[](int index){
        return data->get(index);
    }
    PVector<T> undo()
    {
        if((*data).length==0) 
        {
            cout<<"cannot undo"<<endl;
            return *this;
        }
        return PVector<T>((*data).point_);
    }

    PVector<T> update(const PVector& b)
    {
        int depth_of_a=0,depth_of_b=0;
        shared_ptr<Data<T>> Dpointa,Dpointb;
        for(Dpointa=data;(*Dpointa).length!=0;)
        {
            ++depth_of_a;
            Dpointa=(*Dpointa).point_;
        }
        for(Dpointb=b.data;(*Dpointb).length!=0;)
        {
            ++depth_of_b;
            Dpointa=(*Dpointb).point_;
        }
        Dpointa=data;Dpointb=b.data;
        bool origin=false;int i=0,j=0;
        while(i!=depth_of_a||j!=depth_of_b)
        {
            if(Dpointa==Dpointb) {origin=true;break;}
            Dpointb=(*Dpointb).point_;
             ++j;
            if(Dpointa==Dpointb) {origin=true;break;}
            if(j==depth_of_b&&i==depth_of_a) {break;}
            if(j==depth_of_b) {j=0;Dpointb=b.data;Dpointa=(*Dpointa).point_;++i;}
        }
        if(origin==false) 
        {
            cout<<"cannot update: no origin found";
            return *this;
        }
        //Not to be continued(vector)
    }
};

template<class T>
ostream& operator<<(ostream& out,const PVector<T>& vec)
{
    out<<"[";
    for(int i=0;i<vec.data->length;++i)
    {
        out<<vec.data->get(i);
        if(i!=vec.data->length-1)
        {
            out<<",";
        }
    }
    out<<"]";
    return out;
}