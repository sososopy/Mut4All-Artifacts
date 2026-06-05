
template<typename T>
struct A {
    int test(T* Ptr) {
        return Ptr->type::test();
    }
};

