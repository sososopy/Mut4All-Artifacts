template<typename> concept False = false;

template <class T> 
struct Foo { 
    template <class U>
    requires False<U>
    Foo(T, U);
};

Foo abc(1, 2); // CTAD fails