
void f(unsigned);

template<unsigned n>
void g()
{
    for (unsigned i = 0; i < n; i++) {
        f(i);
    }
}

void h()
{
    g<0>();
}
