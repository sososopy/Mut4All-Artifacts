

struct A {
    typedef int& reference;
};

void f(const A::reference);          // OK
void g(const typename A::reference); // error: 'const' qualifiers cannot be 
                                     // applied to 'A::reference {aka int&}'


