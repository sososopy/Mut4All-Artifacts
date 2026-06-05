
template <class T> struct S : T {};
template <class T> concept bool ConcreteDerivableFrom()
{
    return requires()
    {
        S<T>();
    };
}

struct X final {};

template <ConcreteDerivableFrom C>
void f() {}

template <class T>
void f() {}

int main()
{
    f<X>();
}

