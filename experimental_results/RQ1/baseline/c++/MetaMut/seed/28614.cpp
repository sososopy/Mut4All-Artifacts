
template <int N>
struct LiteralResolver {
	constexpr LiteralResolver(const char (&text)[N]) {
	}
    using type = int;
};
template <LiteralResolver Resolve>
using q_literal_type = decltype(Resolve)::type;
template <LiteralResolver Resolve>
q_literal_type<Resolve> operator""_q() {
}
int main() {
    "hello"_q;
}

