template<class A> struct X
{
    template<class B> struct Base;
    template<class B> struct Data;
};

template<class A>
template<class B>
struct X<A>::Data : X<A>::Base<B>
{ };


