
#include <iostream>

template<typename T>
void call(T&& fn)
{    
    std::cout<<typeid(decltype(fn)).name()<<"\n";
}

[[gnu::stdcall]] bool test(void* eios) noexcept
{    
    return false;
}

int main() {
    call(test);
    return 0;
}
