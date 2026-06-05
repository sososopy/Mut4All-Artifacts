
template<class F> struct S{S(F&&f){}}; 
int main()
{
  S([]{});
}

//explicit deduction via a helper function works as expected
template<class F> struct S{S(F&&f){}}; 
template<class F> auto H(F&& f){return S<F>(forward<F&&>(f));}
int main()
{
  H([]{});
}
