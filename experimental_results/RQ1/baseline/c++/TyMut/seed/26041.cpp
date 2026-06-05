

template<typename _Tp> class complex;
template<>     
struct complex<double>
{
  typedef __complex__ double _ComplexT;
  constexpr complex(double __r = 0.0, double __i = 0.0)
      : _M_value{ __r, __i } { }
  _ComplexT _M_value;
};
template <typename Tp> 
struct B
{
  B() { complex<double>((true ? 1.0 : complex<double>())); } 
};
