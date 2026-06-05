
 
#include <type_traits>

extern void* enabler; 

template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type*& = enabler>
class A
{
public:
    A()
    {}
    template <typename U>
    A& operator=( A<U>&& rhs )
    {
        return *this;
    }
};

int main()
{
    A<int> a_i;
    A<double> a_d;

    a_i = a_d;
}

