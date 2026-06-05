template <typename _Tp, _Tp __v> struct integral_constant {
  static constexpr _Tp value = __v;
};
template <bool __v> using bool_constant = integral_constant<bool, __v>;
template <typename> struct conditional;
template <typename...> struct __and_;
template <typename _B1, typename _B2>
struct __and_<_B1, _B2> : conditional<_B2>::type {};
template <typename _Tp, typename _Up = _Tp> _Up __declval(int);
template <typename _Tp> auto declval() -> decltype(__declval<_Tp>(0));
template <typename _Iftrue> struct conditional { typedef _Iftrue type; };
template <typename> using void_t = void;
template <typename> struct iterator_traits;
template <typename _Tp> struct iterator_traits<_Tp *> {
  typedef _Tp reference;
};
struct pointer_traits {
  template <typename _Up> using rebind = _Up *;
};
template <typename _Iterator> class __normal_iterator {
public:
  typename iterator_traits<_Iterator>::reference operator*();
};
template <typename _Tp> class allocator {
public:
  typedef _Tp value_type;
};
template <typename _Alloc> struct allocator_traits {
  template <typename _Tp> struct _Ptr {
    using type = pointer_traits::rebind<_Tp>;
  };
  using const_pointer = typename _Ptr<typename _Alloc::value_type>::type;
};
template <typename _Alloc> struct _Vector_base {
  typedef allocator_traits<_Alloc> _Tp_alloc_type;
};
template <typename _Tp, typename _Alloc = allocator<_Tp>> class vector {
  typedef typename _Vector_base<_Alloc>::_Tp_alloc_type _Alloc_traits;
public:
  __normal_iterator<typename _Alloc_traits::const_pointer> begin() const;
};
template <class, class, template <class> class, class...> struct detector;
template <class Default, template <class> class Op, class... Args>
struct detector<Default, void_t<Op<Args...>>, Op, Args...> {
  using value_t = integral_constant<bool, true>;
};
template <template <class> class Op, class... Args>
using is_detected = typename detector<int, void, Op, Args...>::value_t;
template <bool... Bs>
constexpr bool require = __and_<bool_constant<Bs>...>::value;
template <typename T, typename R, template <class...> class M,
          typename... Arguments>
constexpr bool has_method = M<T, R, Arguments...>::template tv<T>::value;
template <class T, typename, typename... Arguments> struct time_t {
  template <typename T_> struct fptr_meta {
    template <typename... Arguments_>
    using type =
        integral_constant<decltype(T_().time(declval<Arguments_>()...)) (T_::*)(
                              Arguments_...) const,
                          &T_::time>;
  };
  template <typename T_, typename... Arguments_>
  using fptr_meta_t = typename fptr_meta<T_>::template type<Arguments_...>;
  template <typename> struct tv {
    static constexpr bool value = is_detected<fptr_meta_t, T, Arguments...>::value;
  };
};
template <class T, typename, typename... Arguments> struct output_step_size_t {
  template <typename T_> struct fptr_meta {
    template <typename... Arguments_>
    using type =
        integral_constant<decltype(T_().outputStepSize(
                              declval<Arguments_>()...)) (T_::*)(Arguments_...)
                              const,
                          &T_::outputStepSize>;
  };
  template <typename T_, typename... Arguments_>
  using fptr_meta_t = typename fptr_meta<T_>::template type<Arguments_...>;
  template <typename> struct tv {
    static constexpr bool value = is_detected<fptr_meta_t, T, Arguments...>::value;
  };
};
template <typename S, typename state = typename S::State>
struct Trans_NS_Stepper_StepperConcept {
  constexpr static bool time_exists =
      has_method<S, double, time_t, const state &>;
  constexpr static bool output_step_size_exists =
      has_method<S, char, output_step_size_t, const state &>;
  constexpr static bool value = require<time_exists, output_step_size_exists>;
};
template <typename stepper>
constexpr bool StepperConcept = Trans_NS_Stepper_StepperConcept<stepper>::value;
template <typename stepper_t> class Propagator {
  static_assert(StepperConcept<stepper_t>);
};
template <typename = int> class EigenStepper {
public:
  struct State {};
  void time(State);
};
template <typename _InputIterator, typename _Tp, typename _BinaryOperation>
void accumulate(_InputIterator __first, _Tp __init,
                _BinaryOperation __binary_op) {
  __binary_op(__init, *__first);
}
template <typename extensionlist_t = int>
class MultiEigenStepper : EigenStepper<> {
public:
  using SingleStepper = EigenStepper<extensionlist_t>;
  using SingleState = State;
  struct State {
    struct Component {
      SingleState state;
    };
    vector<Component> components;
  };
  void time(const State &state) const {
    accumulate(state.components.begin(), 0.0,
               [&](auto, auto cmp) { SingleStepper::time(cmp.state); });
  }
  void outputStepSize(const State &) const;
};
using MultiStepper = MultiEigenStepper<>;
using MultiPropagator = Propagator<MultiStepper>;
MultiPropagator makePropagator() { return {}; }

