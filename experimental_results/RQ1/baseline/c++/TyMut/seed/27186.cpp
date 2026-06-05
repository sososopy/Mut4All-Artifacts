
template<class S>
struct A {
    template<class T> void fn();
    struct B { void init(A *p); };
};

template<class S>
void A<S>::B::init(A *p) {
    [p](){ p->fn<int>(); }; // BUG
    // [p](){ p->A::fn<int>(); }; // OK
    // [=](){ p->fn<int>(); }; // OK
}

int main(){}

