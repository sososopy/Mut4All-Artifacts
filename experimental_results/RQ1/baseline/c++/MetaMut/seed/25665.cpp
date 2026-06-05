
template <class T>
class valarray {
  void bar();
};
extern template void valarray<int>::bar();
