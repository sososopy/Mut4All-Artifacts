#include <iostream>
struct A{
    A() = default;
    A(A const&){}
};
struct B:A{};
struct C{
    explicit operator B(){
        return B{};
    }
};

int main(){
   C c;
   A a(c);  // #1
}

