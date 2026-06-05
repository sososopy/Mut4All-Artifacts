
template <typename> struct integer_sequence;
template <long... _Idx> using index_sequence = integer_sequence<long, _Idx...>;
struct QNonConstOverload {
  template <typename R, typename T>
  constexpr auto operator()(R(T::*ptr)) -> decltype(ptr) {
    return ptr;
  }
};
template <typename...> QNonConstOverload qOverload;
struct make_index_sequence_helper {
  using result = index_sequence<>;
};
template <int> using make_index_sequence = make_index_sequence_helper::result;
template <int N> using StaticStringArray = const char[N];
template <int N, typename = make_index_sequence<N>> struct StaticString;
template <int N, long... I> struct StaticString<N, index_sequence<I...>> {
  constexpr StaticString(StaticStringArray<N>);
};
struct {
} W_EmptyFlag;
template <typename F, int NameLength> struct MetaMethodInfo {
  F func;
  StaticString<NameLength> paramTypes;
};
template <typename F, int N, typename ParamTypes>
constexpr MetaMethodInfo<F, N> makeMetaSlotInfo(F f, StaticStringArray<N> &name,
                                                ParamTypes) {
  return {f, name};
}
class SlotTutorial {
  using W_ThisType = SlotTutorial;
  void overload() {
    MetaMethodInfo<void (SlotTutorial::*)(), 9> __trans_tmp_3 =
        makeMetaSlotInfo(qOverload<>(&W_ThisType::overload), "overload",
                         W_EmptyFlag)
  }
};

