

#include <new>

int main()
{
    char buf[256];
    std::size_t n = 10;
    int* p = new (buf) (int[n]);  // incorrect way, parenthesis by mistake
    // int* p = new (buf) int[n]; // correct way
}

