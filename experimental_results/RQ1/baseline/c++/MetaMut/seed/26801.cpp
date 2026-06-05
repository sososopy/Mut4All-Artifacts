

template<class T> auto foo(T t) {return 3;}
class B {B();};
template<class T> class D:public B
{
    D()
    {
        T x00;
        foo(x00);
    }
};
D<int> d;

