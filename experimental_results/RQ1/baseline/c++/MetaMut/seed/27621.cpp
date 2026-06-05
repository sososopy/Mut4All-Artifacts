
#include <type_traits>
using namespace std;
struct yesthrow_t
{
    yesthrow_t()                              noexcept(false) = default;
    yesthrow_t(const yesthrow_t&)             noexcept(false) = default;
    yesthrow_t(      yesthrow_t&&)            noexcept(false) = default;
    yesthrow_t& operator=(const yesthrow_t&)  noexcept(false) = default;
    yesthrow_t& operator=(      yesthrow_t&&) noexcept(false) = default;
};
static_assert(!is_nothrow_default_constructible_v<yesthrow_t>);
static_assert(!is_nothrow_copy_constructible_v<   yesthrow_t>);
static_assert(!is_nothrow_copy_assignable_v<      yesthrow_t>);
static_assert(!is_nothrow_move_constructible_v<   yesthrow_t>);
static_assert(!is_nothrow_move_assignable_v<      yesthrow_t>);
