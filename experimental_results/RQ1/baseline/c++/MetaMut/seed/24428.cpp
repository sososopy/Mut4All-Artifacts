

#include <memory>

class A {
    std::unique_ptr<int> ptr_;
public:
    A() : ptr_(new int(123))
    { }
    A(A&& other) : ptr_(std::move(other.ptr_))
    { }
};

void f(A a)
{ }

void g(A a = {}) // replace "{}" with "A()" makes it work
{ f(std::move(a)); }

int main()
{ g(); }


