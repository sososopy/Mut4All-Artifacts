
#include <type_traits>

template <typename>
struct S
{
    ~S()
    requires(std::is_rvalue_reference(*this));
};

S<int> s{};

