
template <class T>
struct foo {
    // the issue is const here
    void bar(T& foobar) const { foobar = 0; }
};

template <> void
foo<int>::bar(int& foobar) { foobar = 9; }
