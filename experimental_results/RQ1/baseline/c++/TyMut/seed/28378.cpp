
template <class Lhs, class Rhs>
concept Ord = requires(Lhs lhs, Rhs rhs) { lhs <=> rhs; };
template <class> struct S;
template <class T, class U>
  requires(Ord<T *, U *>)
auto operator<=>(T, U) {
  !Ord<int, S<int>>;
}
