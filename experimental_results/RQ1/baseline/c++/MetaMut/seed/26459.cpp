
struct X {
    constexpr X() { }
    constexpr ~X() { }
};

int main()
{
    static constinit X data;
}

