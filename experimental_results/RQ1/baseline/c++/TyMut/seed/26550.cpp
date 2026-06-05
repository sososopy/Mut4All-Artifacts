#include <iostream>

struct A{
    void foo(){}
};

A operator ""_to(const char* ptr){
    std::cout<< ptr;
    return A{};
}

int main(){
    123_to. foo();
}
