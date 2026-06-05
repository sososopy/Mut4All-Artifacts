The following C++11 code causes an ICE when compiled with the current GCC trunk on x86_64-linux-gnu in both 32-bit and 64-bit modes.  

This is a regression from 6.1.x.


template < typename > struct A
{
  A *a;
  template < int > using T = decltype (a);
  template < int C > T < C > foo ();
};
