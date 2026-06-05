
#ifdef WORKAROUND
struct unusable{};
void foo(unusable);
#endif

namespace NX
{
struct X {};
void foo(X){}
}

namespace NY
{
struct Y {};
}


template<class T>
auto ADLfoo(T&&x)  -> decltype((foo(T{}), short())); //decltype(foo(x));


char ADLfoo(...);

static_assert(sizeof(ADLfoo(NY::Y{})) == 1, "");
static_assert(sizeof(ADLfoo(NX::X{})) == 2, "");

int main(){}

