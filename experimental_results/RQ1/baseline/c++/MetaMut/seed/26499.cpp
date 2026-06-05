
template <class T>
concept Foo = requires(T val) {
  val == val;
};

template <class T>
struct Bar {};
template <class T>
bool operator==(const Bar<T>&, const Bar<T>&);

template <class T>
requires Foo<Bar<T>>
bool operator==(const Bar<T>&, int);

static_assert(Foo<Bar<int>>);
