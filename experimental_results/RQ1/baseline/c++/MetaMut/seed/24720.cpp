
template<class T> struct A { };
template<class S,class T> struct B
{
    A<int> ddx;
    template<class U,class V> decltype(f(A<U>(),ddx)) operator-(B<U,V>) const;
};
B<int,int> f(A<int>,A<int>);
B<int,int> x,y=x-x;

