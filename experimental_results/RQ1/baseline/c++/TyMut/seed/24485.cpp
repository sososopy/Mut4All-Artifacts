
struct foo {
    // also ICEs if not static
    static decltype(auto) bar()
    { return 42; }
};

int main()
{
}
