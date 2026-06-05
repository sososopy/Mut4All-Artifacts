
template<typename D> struct complex { };

template<typename Tp>
inline complex<Tp>
pow(const complex<Tp>& x, const complex<Tp>& y)
{ return x; }

template<typename T, typename U>
struct promote_2
{
    typedef T type;
};

template<typename Tp, typename Up>
inline complex<typename promote_2<Tp, Up>::type>
pow(const complex<Tp>& x, const complex<Up>& y)
{ return x; }


complex<double> (*powcc)(const complex<double>&, const complex<double>&) = pow;
