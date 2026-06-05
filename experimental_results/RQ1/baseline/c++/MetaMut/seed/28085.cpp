
#include <array>

template<typename T>
struct debug_t;

template<typename T>
struct A{
/*use the primary template will cause the error*/
static void _() {debug_t<T> d;}
};

template<typename T,int N>
struct A<std::array<T,N>>
{
static void _() {}
};

int main()
{
A<std::array<int,2>>::_();
}
