
    #include <functional>
    #include <iostream>

    template<typename F>
    void foo(F fn, std::function<void(void)> = [](){}) {
        fn();
    }

    int main() {
        foo([](){ std::cout << "First" << std::endl;});
        foo([](){ std::cout << "Second" << std::endl;});
    }


