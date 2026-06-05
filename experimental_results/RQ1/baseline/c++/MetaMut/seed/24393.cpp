
#include <iostream>

template <int w>
void f(){
    for (int i=-1*static_cast<int>(w); i <= w; ++i)
        std::cout << i << std::endl;
}

int main()
{
    f<3>();
    return 0;
}

