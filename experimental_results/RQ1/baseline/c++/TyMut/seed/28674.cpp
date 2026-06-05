
template<class T>
concept C_bug_with_forward_decl = requires(T& t){
    t.template f<class S>();
};

struct good {
    template<class T> auto f() -> void {}
};

static_assert(C_bug_with_forward_decl<good>); // works in GCC 10.3.0, fails on GCC 11 onward

