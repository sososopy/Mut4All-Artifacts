
#include <stddef.h>  

struct foo {
      int member;
};

template < int N>
struct bar {};

template <int N>
struct qux {
        static bar<N+offsetof(foo,member)> static_member;
};

template <int N>
bar<N+offsetof(foo,member)> qux<N>::static_member;

int main() { }

