
#include <vector>

template <typename Iterator>
void fn(Iterator begin, Iterator end)
{
  auto x = [](const decltype(*begin) b) { };
}

void foo(std::vector<int> &vi)
{
  fn(vi.begin(), vi.end());
}

void bar(const std::vector<int>& vi)
{
  fn(vi.begin(), vi.end());
}
