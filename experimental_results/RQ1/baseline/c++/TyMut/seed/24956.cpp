
template<typename T>
concept bool C = requires(T t) {
    { +t }
};  // expected error: expected ‘;’ before ‘}’ token
static_assert(C<int>);

