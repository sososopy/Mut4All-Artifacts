
#include <iostream>

template <class X> 
struct s
{
        template <class T> 
        static constexpr T f1(const T x) {return x;}
        template <class T, T = f1<T>(sizeof(T))> 
        static constexpr T f2(const T x) {return x;} 
        static void f() {s<int>::f2(42);}
};

int main()
{
    s<int>::f();
}
