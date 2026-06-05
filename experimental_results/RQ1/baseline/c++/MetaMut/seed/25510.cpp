
template<int x>
struct S {};

template<long long i>
void g(S<i>);

void f()
{
    S<1000> t;
    g(t);
}

