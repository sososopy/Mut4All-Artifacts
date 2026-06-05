
enum class Ordering { SMALLER = -1, EQUAL, LARGER };
template <typename T>
Ordering compare_vfunc(T t) {
  return Ordering{compare(t)};
}

