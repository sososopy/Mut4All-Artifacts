
template <bool, typename _Tp = void> struct enable_if {};

template <typename _Tp> struct enable_if<true, _Tp> {
  typedef _Tp type;
};

template <typename T> using Invoke = typename T::type;

template <typename Condition, typename T>
using RetEnableIf = Invoke<enable_if<Condition::value, T>>;
template <typename Condition, typename T>
using RetEnableIf = Invoke<enable_if<!Condition::value, T>>;
