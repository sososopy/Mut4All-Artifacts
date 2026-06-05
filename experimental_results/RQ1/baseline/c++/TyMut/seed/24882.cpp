#include <type_traits>
#include <utility>

struct trivially_copyable_no_assignment {
  int arr[4];
  trivially_copyable_no_assignment& operator=(const trivially_copyable_no_assignment&) = delete;
};

static_assert(std::is_trivially_copyable<std::pair<trivially_copyable_no_assignment, int> >::value, "");

int main() { }
