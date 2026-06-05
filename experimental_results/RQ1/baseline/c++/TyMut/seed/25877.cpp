
#include <iostream>

int main()
{
    auto f = []() {
        static int x = 0;
        return [&]() -> void {
            ++x;
            std::cout << x << std::endl;
        };
    }();
    
    f();
}
