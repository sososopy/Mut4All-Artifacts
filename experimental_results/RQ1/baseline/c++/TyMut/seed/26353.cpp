
template<int L, typename T>
inline constexpr bool templ = false;

template<typename T>
void ice(T a)
{
    auto aa = a;
    auto lambda = []<int I>(){
        if constexpr (templ<I, decltype(aa)>) {}
    };
    lambda.template operator()<0>();
}

void go() {
    ice(0);
}

