
#include <iostream>

int main (int, char*[])
{
    for (int i = 0; i < 4; ++i){
        int q = i * static_cast<int>(1000000000u);
        std::cout << q << std::endl;
    }
    return 0;
}
