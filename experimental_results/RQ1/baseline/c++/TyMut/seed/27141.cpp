
template<typename T>
struct Test {
    ~Test() requires true {}
    ~Test() requires true && true {}
};

Test<int> t;

