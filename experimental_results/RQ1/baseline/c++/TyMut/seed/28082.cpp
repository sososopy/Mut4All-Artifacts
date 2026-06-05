
template <int...> struct seq {};
template <bool> struct S {
    template <typename Args>
    constexpr static void call(Args&&...) {}
};

template <int ...Idx,typename ...Args>
auto foo (seq<Idx...>, Args&& ...args) {
    return (S<Idx==sizeof...(args)>::call(args), ...);
}

void bar() {
    foo(seq<0,1,2>{}, 1,2,3);
}

