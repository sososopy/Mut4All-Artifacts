


template < int > struct A;

template < typename ... T >
struct B
{ 
  template < typename A < T::value >::type > void foo ();
};

int main ()
{ 
  B < int > t;
  t.foo ();
  return 0;
}


