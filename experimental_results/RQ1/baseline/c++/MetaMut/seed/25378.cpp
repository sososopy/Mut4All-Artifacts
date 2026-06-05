
struct one {
    int i;
};

int main()
{
    auto&& [i] = one {};
    // error: taking address of xvalue (rvalue reference)
    &i;
}
