template<typename>
struct S {
    static constexpr int arr[] = {1,2,3};
};

constexpr int foo( int x ) {
    return S<void>::arr[x];
}

constexpr int x = foo(1);