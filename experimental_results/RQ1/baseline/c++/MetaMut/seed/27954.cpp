namespace N {
    template <typename T>
    struct X{ X(int); };
}

using N::X;

X(int) -> X<int>;

