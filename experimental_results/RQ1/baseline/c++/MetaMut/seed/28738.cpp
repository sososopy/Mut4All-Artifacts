namespace A
{
    struct B {};
    using C = B;
}

void f(A::B&);
void f(A::C&);

void g(const A::B& b, const A::C& c) {
    f(b);
    f(c);
}
