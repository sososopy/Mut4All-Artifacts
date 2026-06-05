
#include <functional>
#include <memory>

template<class T>
struct unique {
    auto operator()(auto&&... args) {
        return std::make_unique<T>(std::forward<decltype(args)>(args)...);
    }
};

template<template<class...> class T, class... Args>
using deduced_type = decltype(T{std::declval<Args>()...});

template<template<class> class F, template<class...> class T, class... Args>
auto make(Args&&... args) {
    return F<deduced_type<T, Args...>>{}(std::forward<Args>(args)...);
}

template<class A, class B>
struct Foo { Foo(A,B) {} };


int main(){
    auto foo_unique = make<unique, Foo>(1, 2);
}

