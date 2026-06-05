

// should be: template < decltype (0) > struct A
template < typename decltype (0) > struct A
{ 
  void foo () { baz (); }
  template < typename ... S > void baz () {};
};
