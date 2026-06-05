
#include <compare>

struct Legacy
{
  bool operator==(Legacy const&) const;
  bool operator<(Legacy const&) const;
};

struct A
{
  std::strong_ordering operator <=> (A const& other) const = default;
  Legacy l;
};

int main()
{
  A a, b;

  return a < b;    
}
