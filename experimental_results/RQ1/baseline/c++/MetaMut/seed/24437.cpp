
#include <iostream>
using namespace std;

struct HasIter {
   typedef int * const_iterator;
};

struct NoIter {
};

template <typename T>
void foo(const T &, typename T::const_iterator *)
{
   cout << "has iterator" << endl;
}

template <typename T>
void foo(const T &, ...)
{
   cout << "no iterator" << endl;
}

int main()
{
   HasIter has_iter;
   NoIter no_iter;
   foo(no_iter, 0);
   foo(has_iter, 0);
   foo<HasIter>(has_iter, 0);
}

