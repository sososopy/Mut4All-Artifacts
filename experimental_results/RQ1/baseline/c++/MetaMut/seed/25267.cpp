
template < typename, typename > struct A {};

template < typename ... T > struct B
{ 
  template < typename > struct C
  { 
    static const int a = 0;
  };

  template < typename R, typename ... S >
  struct C < R (A < T, S > ...) >
  { 
    static const int a = 1;
  };
};

template <> const int B <>::template C < int () >::a;
