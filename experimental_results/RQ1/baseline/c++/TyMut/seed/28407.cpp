
#include <memory>
#include <cstdint>
#include <functional>

template<class T>
struct A{
    using Callback = std::function<std::size_t(std::size_t)>; 

    A(Callback pol = [](auto){return 42;}){
        pol(1);
    }
};

struct B{
    B() = default;

    B(int){}

    A<std::uint32_t> member;
};

struct C{
    void foo() {
        auto ptr = std::make_unique<B>();
    } 
};
