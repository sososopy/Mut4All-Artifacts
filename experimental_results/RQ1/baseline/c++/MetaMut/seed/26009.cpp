
#include <iostream>
#include <type_traits>

template<class T>
void writeln(T const& t){   std::cout << t << std::endl; }

namespace vargs
{
template< class ...T>
using size_of = std::integral_constant< decltype(sizeof...(T)), (sizeof...(T) )>;
}


namespace mpl
{
    template<class ...T>
    struct tuple
    {
        using type = tuple;
    };
    
    template< class TP> struct size_of;
    
    template< class ...T> struct size_of< tuple<T...> > : vargs::size_of<T...> {};
}

int main(int argc, char * argv[])
{
    auto n = vargs::size_of<int,float,double>::value;
    
    writeln(n);
    
    typedef mpl::tuple<int,float,double,char> mytuple;
    
    auto s = mpl::size_of<mytuple>::value;
    
    writeln(s);
}
