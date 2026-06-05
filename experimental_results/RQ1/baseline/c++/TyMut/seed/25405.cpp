

template < typename S > struct A
{ 
  template < typename T > static auto f () { return 0; }
  template < class U = decltype (f < S > ()) > int g () { return 0; }
};

auto a = A < int > {}.g ();
