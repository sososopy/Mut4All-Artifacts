

template < class > struct S
{ 
  template < class > struct A
  { 
    template < class > struct B;
  };

  template < class X >
  template < class Y >
  template < class Z >
  friend void A < X >::B < Y >::operator= (Z);
};

S < int > s;
