
struct A { A(); };
struct B { B(A); };

struct composed2 {
    B b_;
    A a_;
    composed2() : b_(a_)  {}
};


