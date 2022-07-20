#pragma once
#include <algorithm>
#include <vector>
#include <tuple>
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
    virtual const T& get_index(int index)=0;
};

template<class T>
class DataEmpty : public Data<T>
{
public:
    DataEmpty<T>(): Data<T>(0) {};
    const T& get_index(int index) {
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
    const T& get_index(int index)
    {
        if(_index==index) return end;
        else{return (*(this->point_)).get_index(index);}
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
    const T& get_index(int index)
    {
        if(_index==index) return _value;
        else{return (*(this->point_)).get_index(index);}
    }
};

template<class T>
class DataUpdateDecorator : public Data<T>
{
    public:
    /*
    * A better way to implement: 
    * container_ stores the Manage Pointers pointing to tuple<int,T,bool>
    * which could avoid the repeated construction of T
    * 
    * vector<shared_ptr<tuple<int,T,bool>>> container_
    */
    vector<tuple<int,T,bool>> container_;
    DataUpdateDecorator(shared_ptr<Data<T>> data,vector<tuple<int,T,bool>> container):Data<T>(data->length)
    {
       container_=container;
       this->point_=data;
       /*
       * The index of push_back elements should be replaced with new one and calculated reversely
       */
       for(int i=container.size()-1;i>=0;--i)
       {
           if (get<2>(container[i])) { get<0>(container[i]) = this->length; ++this->length; }
           //cout << "the " << i << " number is： " << get<0>(container[i]) << " " << get<1>(container[i]) << " " << get<2>(container[i]) << endl;
       }
    }
    const T& get_index(int index)
    {
        for(int i=0;i<container_.size();++i)
        {
            if(get<0>(container_[i])==index) return get<1>(container_[i]);
        }
        return (*(this->point_)).get_index(index);
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
        return data->get_index(index);
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
            Dpointb=(*Dpointb).point_;
        }
        //cout << "depth " << depth_of_a << " " << depth_of_b << endl;
        Dpointa=this->data;Dpointb=b.data;
        bool origin=false;int i=0,j=0;
        while(i<=depth_of_a||j<=depth_of_b)
        {
            if(Dpointa==Dpointb) {origin=true;break;}
            if (j == depth_of_b && i == depth_of_a) {break;}
            if (j == depth_of_b) { j = 0; Dpointb = b.data; Dpointa = (*Dpointa).point_; ++i; continue; }
            Dpointb=(*Dpointb).point_;
             ++j;
            if(Dpointa==Dpointb) {origin=true;break;}
            if(j==depth_of_b&&i==depth_of_a) {break;}
        }
        if(origin==false) 
        {
            cout<<"cannot update: no origin found"<<endl;
            return *this;
        }
        //to be continued(vector needed)
        else
        {
            shared_ptr<Data<T>> anc= Dpointa;
            Dpointa=data,Dpointb=b.data;
            vector<tuple<int,T,bool>> container_a,container_b;
            bool pu_counta=false,pu_countb=false;
            while (Dpointa!=anc)
            {
                /*
                * Once written as dynamic_pointer_cast<make_shared<DataPushBackDecorator<T>>>(Dpointa) and cause some trouble
                */
                if(dynamic_pointer_cast<DataPushBackDecorator<T>>(Dpointa))
                {
                    container_a.push_back(make_tuple((*dynamic_pointer_cast<DataPushBackDecorator<T>>(Dpointa))._index,(*dynamic_pointer_cast<DataPushBackDecorator<T>>(Dpointa)).end,true));
                    //cout << "pushback " << "index " << (*dynamic_pointer_cast<DataPushBackDecorator<T>>(Dpointa))._index << " value " << (*dynamic_pointer_cast<DataPushBackDecorator<T>>(Dpointa)).end << endl;
                }
                else if(dynamic_pointer_cast<DataSetDecorator<T>>(Dpointa))
                {
                    container_a.push_back(make_tuple((*dynamic_pointer_cast<DataSetDecorator<T>>(Dpointa))._index,(*dynamic_pointer_cast<DataSetDecorator<T>>(Dpointa))._value,false));
                }
                else if(dynamic_pointer_cast<DataUpdateDecorator<T>>(Dpointa))
                {
                    container_a.insert(container_a.end(),(*dynamic_pointer_cast<DataUpdateDecorator<T>>(Dpointa)).container_.begin(),(*dynamic_pointer_cast<DataUpdateDecorator<T>>(Dpointa)).container_.end());
                }
                else{cout<<"Error in container_a";break;}
                Dpointa=(*Dpointa).point_;   
            }
            while (Dpointb!=anc)
            {
                if(dynamic_pointer_cast<DataPushBackDecorator<T>>(Dpointb))
                {
                    container_b.push_back(make_tuple((*dynamic_pointer_cast<DataPushBackDecorator<T>>(Dpointb))._index,(*dynamic_pointer_cast<DataPushBackDecorator<T>>(Dpointb)).end,true));
                    //cout << "pushback " << "index " << (*dynamic_pointer_cast<DataPushBackDecorator<T>>(Dpointb))._index << " value " << (*dynamic_pointer_cast<DataPushBackDecorator<T>>(Dpointb)).end << endl;
                }
                else if(dynamic_pointer_cast<DataSetDecorator<T>>(Dpointb))
                {
                    container_b.push_back(make_tuple((*dynamic_pointer_cast<DataSetDecorator<T>>(Dpointb))._index,(*dynamic_pointer_cast<DataSetDecorator<T>>(Dpointb))._value,false));
                }
                else if(dynamic_pointer_cast<DataUpdateDecorator<T>>(Dpointb))
                {
                    container_b.insert(container_b.end(),(*dynamic_pointer_cast<DataUpdateDecorator<T>>(Dpointb)).container_.begin(),(*dynamic_pointer_cast<DataUpdateDecorator<T>>(Dpointb)).container_.end());
                }
                else{cout<<"Error in container_b";break;}
                Dpointb=(*Dpointb).point_;        
            }
            bool wr=false;
            for(int i=0;i<container_a.size();++i)
            {
                if(get<2>(container_a[i])==true) pu_counta=true;
                for(int j=0;j<container_b.size();++j)
                {
                    if(get<0>(container_a[i])==get<0>(container_b[j])&& get<2>(container_b[j]) == false&& get<2>(container_b[j])==false) {wr=true;break;}
                    if(get<2>(container_b[j])==true) pu_countb=true;
                }
                if(pu_counta==true && pu_countb==true) {wr=true;break;}
            }
            if(wr) {cout<<"cannot update: conflicts found"<<endl;return *this;}
            else {return PVector<T>(make_shared<DataUpdateDecorator<T>>(data, container_b));}
        }
    }
};

template<class T>
ostream& operator<<(ostream& out,const PVector<T>& vec)
{
    out<<"[";
    for(int i=0;i<vec.data->length;++i)
    {
        out<<vec.data->get_index(i);
        if(i!=vec.data->length-1)
        {
            out<<",";
        }
    }
    out<<"]";
    return out;
}