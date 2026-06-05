
#include <type_traits>

class task
{
  friend void suspend_to(auto const tp) noexcept
    requires(std::is_same_v<task* const, decltype(tp)>);
};

class loop
{
  friend void suspend_to(auto const tp) noexcept
    requires(std::is_same_v<task* const, decltype(tp)>);
};

void suspend_to(auto const tp) noexcept
  requires(std::is_same_v<task* const, decltype(tp)>)
{
}

