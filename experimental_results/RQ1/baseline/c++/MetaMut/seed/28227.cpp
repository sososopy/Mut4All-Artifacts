
template<typename T> class X;

inline void f ()
{
  auto l3 = []<typename T>(X<T> *, X<int> *) {};

  l3 ((X<char> *)nullptr, nullptr);
  // _ZZ1fvENKUlTyP1XIT_EPS_IiEE _clIcEEDaS2_S4_  Correct
  // _ZZ1fvENKUl  P1XIT_EPS_IiEE0_clIcEEDaS2_S4_  Wrong
}

void a () 
{
  f ();
}
