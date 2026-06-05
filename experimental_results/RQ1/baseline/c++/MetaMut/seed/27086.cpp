
#include <type_traits>
#include <utility>

using index_t = long;

//------------------------------------------------------------------------------
// repeat_type_c
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <unsigned long long, class T> struct repeat_type_c_impl {
  using type = T;
};
}

template <unsigned long long A, class T>
using repeat_type_c = typename DETAIL_NS::repeat_type_c_impl<A, T>::type;

//------------------------------------------------------------------------------
// uncvref_t
//------------------------------------------------------------------------------
template <class T>
using uncvref_t =
    typename std::remove_cv<typename std::remove_reference<T>::type>::type;

//------------------------------------------------------------------------------
// cpt_Scalar
//------------------------------------------------------------------------------
template <class X> concept bool cpt_Scalar() {
  return std::is_floating_point<X>::value;
}

//------------------------------------------------------------------------------
// cpt_KEvaluator
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class, class> constexpr bool k_evaluator_impl = false;

// clang-format off
template <std::size_t... Indexes, class Evaluator>
  requires requires(const Evaluator& evaluator,
                    repeat_type_c<Indexes, index_t>... indexes) {
    requires cpt_Scalar<uncvref_t<decltype(evaluator(indexes...))>>();
    &Evaluator::operator();
  }
constexpr bool k_evaluator_impl<std::index_sequence<Indexes...>, Evaluator> =
    true;
// clang-format on
}

template <class X, std::size_t K> concept bool cpt_KEvaluator() {
  return DETAIL_NS::k_evaluator_impl<std::make_index_sequence<2 * K>, X>;
}

//------------------------------------------------------------------------------
// cpt_IndexedEvaluatable
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class, class> constexpr bool indexed_evaluatable_impl = false;

// clang-format off
template <std::size_t... Indexes, class Evaluator>
requires requires(const Evaluator& evaluator,
                  repeat_type_c<Indexes, index_t>... indexes) {
          requires cpt_Scalar<uncvref_t<decltype(evaluator(indexes...))>>();
         }
constexpr bool indexed_evaluatable_impl<
                  std::index_sequence<Indexes...>, Evaluator> = true;
// clang-format on
}

template <class Evaluator, std::size_t K>
concept bool cpt_IndexedEvaluatable() {
  return DETAIL_NS::indexed_evaluatable_impl<std::make_index_sequence<2 * K>,
                                             Evaluator>;
}

//------------------------------------------------------------------------------
// make_indexed_evaluator
//------------------------------------------------------------------------------
template <std::size_t K, cpt_IndexedEvaluatable<K> Evaluator>
auto make_indexed_evaluator(const Evaluator& evaluator) {
  return true;
}

int main() {
  auto f1 = [](index_t i, index_t m, index_t j, index_t n) {
    return 2.0;
  };
  auto f1_ = make_indexed_evaluator<2>(f1);
  return 0;
}