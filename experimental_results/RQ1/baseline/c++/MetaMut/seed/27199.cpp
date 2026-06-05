
template<typename>
struct B {
};

template<typename... Types>
struct A : public B<Types>... {
    using B<Types>::operator=...;
    using B<Types>::B...;
};

int main() {}

