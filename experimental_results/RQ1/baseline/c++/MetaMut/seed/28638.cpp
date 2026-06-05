

namespace N
{
template<typename C>
struct string_view
{
  using char_type = C;
};

template<typename C>
struct string
{
  void operator+=(const string&);

  template<typename T, typename = typename T::char_type>
  void operator+=(const T&);
};

template<typename T>
void f()
{
  string<T> s;
  s += string_view<T>();
}

} // namespace N

template<typename T1, typename T2>
void operator+=(T1&, const T2&){}

int main()
{
  N::f<char>();
}

