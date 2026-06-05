
#include <iostream>
#include <tuple>

template <class Type>
struct Getter
{
    template <unsigned int Index = 0, class Tuple>
    static inline Type get(const Tuple& tuple);
};

template <class Type>
template <unsigned int Index, class Tuple>
inline Type Getter<Type>::get(const Tuple& tuple)
{
    return std::get<Index>(tuple);
}

int main(int argc, char* argv[])
{
    std::cout<<Getter<int>::get(std::make_tuple(42))<<std::endl;
    return 0;
}
