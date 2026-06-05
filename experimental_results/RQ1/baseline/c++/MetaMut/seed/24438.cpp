#include <type_traits>

template<typename Type>
constexpr bool IsType = true;

template<class T>
struct X {
    typedef typename std::enable_if<IsType<T>,T>::type type;
};

int main()
{
   X<int>::type t;
}

