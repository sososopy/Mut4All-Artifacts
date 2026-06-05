template<int I> concept C = true;
template<typename T> struct A {
    int f() requires C<42> { return 1; }  // #1
    unsigned f() requires true { return 2u; }  // #2
};
int main() {
    int (A<int>::*p)() = &A<int>::f;
    unsigned (A<int>::*q)() = &A<int>::f;
    return (A<int>().*p)() + (A<int>().*q)();
}
