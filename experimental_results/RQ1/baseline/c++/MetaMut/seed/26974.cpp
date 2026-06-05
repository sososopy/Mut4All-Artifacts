

struct A {};
struct B {};

static auto f(auto b) -> A { return A(); }

int main()
{
    A a = f(B());
    return 0;
}
