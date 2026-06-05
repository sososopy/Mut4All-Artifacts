
void f(const int&);
void f(int&) = delete;

struct A
{
    int i;
    void g() const noexcept(noexcept(f(i)));
};
