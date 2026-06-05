
#include <tuple>
struct ComplexPart;
       
template <typename PA, typename PB> struct FollowParser {
  constexpr FollowParser(PA pa, PB pb) : pa_{pa}, pb_{pb} {}
  const PA pa_;
  const PB pb_;
};
template <typename PA, typename PB>
inline constexpr FollowParser<PA, PB> operator/(PA pa, PB pb) {
  return FollowParser<PA, PB>{pa, pb};
}
template <typename... PARSER> struct ApplyConstructor {
  constexpr ApplyConstructor(PARSER... p) : parsers_{p...} {}
  const std::tuple<PARSER...> parsers_;
};
template <typename... PARSER>
constexpr ApplyConstructor<PARSER...> construct(PARSER... p) {
  return ApplyConstructor<PARSER...>{p...};
}
template <typename A> struct Parser {
  constexpr Parser() {}
};
auto tmp = construct(Parser<ComplexPart>{} / ",", Parser<ComplexPart>{});
