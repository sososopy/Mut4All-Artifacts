
void foo(int) = delete;

template<typename X>
void lookup(X x)
{ foo(x); }

namespace ns {

struct dummy {};
int foo(dummy) { return 3; }

} // ns

int main()
{
    lookup(ns::dummy {});
}
