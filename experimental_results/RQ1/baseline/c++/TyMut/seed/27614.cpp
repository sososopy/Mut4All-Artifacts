

#include <utility>

struct foo {
    void func();
};

void bar(foo& v) {
    [v]() {
        static_assert(std::is_same_v<decltype(v), foo&>);
        [v]() mutable {
            static_assert(std::is_same_v<decltype(v), foo&>);
//            v.func();                                                                                                                                                                                
        }();
    }();
}

