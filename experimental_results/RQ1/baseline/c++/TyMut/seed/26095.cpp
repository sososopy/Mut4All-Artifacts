

#include <iostream>

template<class T> void tf(const T*) {  // 1. clang++ 
    std::cout << "void tf(const T*)\n"; 
}

template<class T> void tf(T) {  // 2. g++ 4.8.1
    std::cout << "void tf(T)\n"; 
}

int main(){ 
    tf(std::hex);
}
