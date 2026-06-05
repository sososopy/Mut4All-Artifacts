
template<typename>
struct A
{
    constexpr A(int) {}
};

A(int) -> A<int>;


template<A> //template<auto> works fine
struct B 
{
    template<typename T>
    B(T);
};

template<typename T>
B(T) -> B<A{T{}}>; //see below


int main()
{
    B t(42);
}

