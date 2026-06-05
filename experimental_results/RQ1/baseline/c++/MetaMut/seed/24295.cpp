
#include <iostream>

template <typename T>
struct is_foo {
    private:
        template<typename U, U> struct helper{};

        template <typename Z> static auto test(Z z) -> decltype(

                helper<void (Z::*)() const,                       &Z::foo>(),
                // All other requirements follow..

                std::true_type()
                );

        template <typename> static auto test(...) -> std::false_type;

    public:
        enum { value = std::is_same<decltype(test<T>(std::declval<T>())),std::true_type>::value };
};

struct A { 
    int foo(); // !!!!!: Failure disappears if this line is removed.
    void foo() const; 
};

struct A1 : public A {};

int main() {
    std::cout << is_foo<A>::value       << '\n';    
    std::cout << is_foo<A1>::value      << '\n';   

    return 0;
}
