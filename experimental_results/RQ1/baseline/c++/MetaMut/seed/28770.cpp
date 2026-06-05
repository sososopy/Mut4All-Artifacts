
template<class _Tp, class = int>
struct vector {
  typedef int allocator_type;
  vector(_Tp, allocator_type = allocator_type());
};
template<class T> using vector_mm = vector<T>;
vector_mm v(0);
