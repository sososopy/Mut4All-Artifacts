
struct A
{
    int i;
};

void g(int& p) {}

template <typename T>
void f()
{
    A a;
    int A::*pm = &A::i;
    g(a.*pm); // (*)
}
