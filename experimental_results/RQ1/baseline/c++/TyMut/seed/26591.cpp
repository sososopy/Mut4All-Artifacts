
template <class T>
using data_type = decltype([](){return 1;}());

template <class T>
auto get_precision(T v) {
  auto tmp = *(data_type<T> *)&v;
  return tmp;
}

