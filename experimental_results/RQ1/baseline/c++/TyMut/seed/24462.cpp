
#include <vector>

template <typename T>
struct Container : public std::vector<int>
{
  typedef std::vector<int> vec;

  void swap(Container& rhs)
  {
    vec::swap(rhs);
  }
};

int main()
{
  Container<int> ca;
  Container<int> cb;

  ca.swap(cb);

  return 0;
}

