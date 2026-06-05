
template<typename T>
concept bool C = requires(T t) {
    requires true
};  // expected error: expected ‘;’ before ‘}’ token
static_assert(C<int>);

