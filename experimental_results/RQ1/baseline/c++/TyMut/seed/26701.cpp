
template <class T>
concept bool C = requires (T t) {
  { *t } -> auto;
};

template <C T>
constexpr bool models = true;

template <class T>
  requires !C<T>
constexpr bool models_not = true;

static_assert(models<int*>);
static_assert(models_not<int*>);
