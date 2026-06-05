
struct S {
    explicit S(const S&) {}
    S(int) {}
};

int main()
{
    S s = 1;
}
