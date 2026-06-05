
#include <utility>

struct move_only
{
  move_only() = default;
  move_only(move_only&&) = default;
  move_only &operator = (move_only&&) = default;
};

struct broken : move_only
{
  move_only mo;
  ~broken() {}
};

void test()
{
  broken a;
  broken b(std::move(a));
}

