
#include <utility>

template <typename U>
auto foo() -> decltype(std::declval<U>() + std::declval<U>());

template <typename T>
decltype(foo<T>()) bar(T) {
//  if ( 0 ) throw;
}

int main()
{ bar(1); }
