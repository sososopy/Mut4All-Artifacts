
template<typename T, bool = true>
struct base;

template<typename T>
struct base<T, true>
{
  base(T& t) : p(&t) { }
  base(const base&) = delete;
  T* p;
};

template<typename T>
struct atomic_ref : base<T>
{
  using base<T>::base;
};

int main()
{
  int i = 0;
  atomic_ref r(i);
}
