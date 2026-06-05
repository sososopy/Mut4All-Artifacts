

struct enclosing {
private:
    template<int N, typename T>
    struct foo {};

    template<typename T>
        requires false
    struct foo<0, T> {};

    template<typename T>
        requires true
    // error: 'struct foo<0, T>' redeclared with different access
    struct foo<0, T> {};
};

int main()
{
}
