
template <class U>
concept bool A =
  requires (U u) { u; };

template <class T>
concept bool B =
  requires (T t) { { t } -> A; };

void foo(B);

