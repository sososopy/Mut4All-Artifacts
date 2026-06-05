
template <int T>
struct S {
    template <class U>
    friend struct S; 
};

S<0> s;

