
template <typename T>
struct A
{
    using type = void;
    
    constexpr static int value = 0;
};

template <typename T>
void func()
{
    typedef A<T> A_typedef;
    using   A_using = A<T>;
    
    int value_typedef = A_typedef::value;
    int value_using   = A_using  ::value;
    
    using type_typedef = typename A_typedef::type;
    using type_using   = typename A_using  ::type;
}

template void func<int>();

