
//test.h

class A{
public:
    virtual const int getA(){} ;
    virtual const int getB(){} ;
};


//testA.cpp

#include<iostream>
#include "test.h"
using namespace std;

class Test : public A {
public:
    virtual const int getA() override;
};
const int Test ::getA() {
    return 1;
}


int main() {
    A * t=new Test();;
    cout << dynamic_cast<Test*>(t);
}

//testB.cpp

#include "test.h"

class Test : A {
public:
    virtual const int getB() override;
};
const int Test :: getB() {
    return 2;
}
