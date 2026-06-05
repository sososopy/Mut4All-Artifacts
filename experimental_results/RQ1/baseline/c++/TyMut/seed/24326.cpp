
struct B { };

template<typename T> struct A : virtual B
{
   A();
   A(const A&);
};

template<typename T> A<T>::A(const A<T>&) = default;

A<int> a = A<int>();

