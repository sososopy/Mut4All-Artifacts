
template<typename T> concept True = true;

template<typename U>
struct S1 {
    template<True T> friend struct S2; // friend declaration for S2
};

S1<int> s; // instantiate S1

template<True T> struct S2; // another declaration for S2
