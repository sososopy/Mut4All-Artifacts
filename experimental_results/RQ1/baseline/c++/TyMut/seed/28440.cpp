#include <iostream> 
#include <cstddef>

void operator delete[]( void* ptr                   ) noexcept {
     std::cout << "====delete with 1 parameters======" << std::endl;
    ::operator delete(ptr);
}

void operator delete[]( void* ptr, std::size_t size ) noexcept {

     std::cout << "====delete with 2 parameters======" << std::endl;
    ::operator delete(ptr);
}

int main()
{
    int* p = new int[2];
    p[0] = 1;
    p[1] = 2;
    delete[] p;
}

