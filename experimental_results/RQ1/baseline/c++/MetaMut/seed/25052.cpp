

#include <iostream>
#include <vector>

using namespace std;

struct A {
    template <typename T>
    void foo(T& f) {
        struct auto_logger {
            ~auto_logger() {
                static thread_local vector<char> b; // Segfault here
            }
        } inst;
    }
};

int main(void) {
    A a;
    vector<char> b;
    a.foo(b);
}

