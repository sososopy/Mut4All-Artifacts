
#include <iostream>
#include <type_traits>
#include <utility>
#include <tuple>
#include <array>

template <class Crtp, class... Types> struct Base
{
    template <unsigned int Index, class Type = typename std::tuple_element<Index, std::tuple<Types...> >::type> inline const Type& get() const {return std::get<Index>(data);}
    template <unsigned int Index, class Type = typename std::tuple_element<Index, std::tuple<Types...> >::type> inline Crtp& set(const Type& value) {std::get<Index>(data) = value; return static_cast<Crtp&>(*this);}
    std::tuple<Types...> data;
};

template <typename Type, unsigned int Size> struct Derived : public Base<Derived<Type, Size>, std::array<Type, Size>>
{
    template <class... Args, class Template = decltype(std::declval<const Base<Derived<Type, Size>, std::array<Type, Size>>>().template get<0>(std::declval<Args>()...))> inline Template test(Args&&... args) const {return this->template get<0>(std::forward<Args>(args)...);} 
    template <class... Args, class Template = decltype(std::declval<const Base<Derived<Type, Size>, std::array<Type, Size>>>().template set<0>(std::declval<Args>()...))> inline Derived<Type, Size>& test(Args&&... args) {return this->template set<0>(std::forward<Args>(args)...);} 
    static void check() {Derived<double, 3> derived; std::cout<<derived.test()[0]<<std::endl;}
};

int main(int argc, char* argv[])
{
    Derived<double, 3> derived; std::cout<<derived.test()[0]<<std::endl; // Working
    Derived<double, 3>::check(); // Not working: error: no match for ‘operator[]’ (operand types are ‘Derived<double, 3u>’ and ‘int’)
    return 0;
}

