
class B
{
 template <typename T> friend struct A;
 B() {}
};

template <typename T>
struct A
{
 A() noexcept(sizeof(B{})) { }
};

struct C
{
 C()
 {
 static_assert( sizeof(A<int>{}), "" );
 }
};
