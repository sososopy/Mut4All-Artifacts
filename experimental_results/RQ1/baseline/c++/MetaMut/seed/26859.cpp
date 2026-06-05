
template <typename>
class A;

template <int>
struct B;

template <typename>
struct C;

template <>
template <int N>
struct C <B <N> >
{
    template<typename T>
    A <B <N> >
    m_fn(T);
};

template <int N>
template <typename T>
A <B <N> >
C <B <N> >::m_fn (T)
{
}

