template<typename T>
  concept anything = true;

template<typename> struct incrementable_traits { using type = int; };

template<typename T>
  requires requires(T a, T b) { { a - b } -> anything; }
  struct incrementable_traits<T>
  {
  };

incrementable_traits<void>::type t;

