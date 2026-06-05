
template <typename... E>
struct T
{
  T(unsigned int i = sizeof...(E)){} // does not compile
    
  static constexpr unsigned int U = sizeof...(E);
  T(unsigned int j, unsigned int i = U){} //  compile
};

template <typename... T>
void test(int i = sizeof...(T)) // compile
{}

