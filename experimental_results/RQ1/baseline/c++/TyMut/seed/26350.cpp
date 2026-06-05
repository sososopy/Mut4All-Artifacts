template<typename L, typename R>
struct fn { using type = decltype(L{} + R{}); };

struct no_self {};
no_self operator+(int, no_self);

void operator+(no_self, no_self) = delete;

template<typename T>
struct expr {
    template<typename U>
        requires requires {typename fn<T, U>::type;}
    friend void operator/(expr<T>, expr<U>);
};

using X = decltype(expr<int>{} / expr<no_self>{});

