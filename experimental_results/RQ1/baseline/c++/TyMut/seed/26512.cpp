
struct A {
    A( float ) {}
    template<class U>
    explicit A( U ) {}
};

void f(A t)
{
  t = {1};
}
