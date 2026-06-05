
template<auto = []{}>
static void f() {}

template<int>
struct bad
{
    template<auto = []{}>
    static void f() {}
};

int main()
{
    f(); //works fine
    bad<0>::f(); //substitution failure
}
