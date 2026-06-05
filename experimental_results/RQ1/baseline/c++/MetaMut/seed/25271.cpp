
template < typename T > struct A
{ 
  template < void (T::*Fn) () > void f () {}
};

template < typename T > struct B : A < B < T > >
{ 
  void g ()
  { 
    this->B::template f < &B < T >::g > ();
  }
};

int main ()
{ 
  B < int > b;
  b.g ();
  return 0;
}
