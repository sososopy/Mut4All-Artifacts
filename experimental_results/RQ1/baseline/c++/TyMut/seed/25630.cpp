
   #include <iostream>
   #include <string>

   using namespace std;


   template<typename A>
   struct X
   {
    template<typename T>
      void f()
      {
        cout << "F" << endl;
      }
  };

  template<template<typename A> class B>
  struct Y
  {
    B<int> xFromB;

    void g()
    {
      X<int> directX;
      directX.f<int>(); // ok

      xFromB.f<int>();  // error: expected primary expresion
    }
  };

  int main(int,char **)
  {
    Y<X> y;
    y.xFromB.f<int>(); // ok

    return 0 ;
  }


