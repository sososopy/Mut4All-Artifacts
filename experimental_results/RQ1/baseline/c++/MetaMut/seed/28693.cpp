
#include <cstdio>
#include <initializer_list>
void f(int x) { printf("%d", x); }
void foo(const std::initializer_list<int> &x) { for (auto y : x) f(y); }
int main()
{
        for (auto x : {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15})
                f(x);
        foo({
        1,2,3,4,5,6,7,8,9,2,1,3,4,5,6,7,8,9,3,2,1,4,5,6,7,8,9,5,4,3,
        1,2,3,4,5,6,7,8,9,2,1,3,4,5,6,7,8,9,3,2,1,4,5,6,7,8,9,5,4,3,
        1,2,3,4,5,6,7,8,9,2,1,3,4,5,6,7,8,9,3,2,1,4,5,6,7,8,9,5,4,3,
        });
}
