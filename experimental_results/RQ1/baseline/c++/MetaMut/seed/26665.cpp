
inline constexpr bool and_impl() { return true; }

template <class OperandFirst, class... OperandsRest>
constexpr bool and_impl(OperandFirst operand_first,
                        OperandsRest... operands_rest) {
  return operand_first && and_impl(operands_rest...);
}

template <class... Operands> constexpr bool and_(Operands... operands) {
  return and_impl(operands...);
}

template <class X> concept bool C() { return true; }

// v1
template<int, class... Xs>
  requires and_(C<Xs>()...)
void f(const Xs&... xs) {
}

// v2
template<int, class... Xs>
void f(const Xs&... xs) {
}

int main() {
  f<10>(3.0, 2.0f);
  return 0;
}
