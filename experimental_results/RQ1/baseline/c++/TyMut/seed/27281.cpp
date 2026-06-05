
#ifndef __clang__
#include <coroutine>
#else
#include <experimental/coroutine>
namespace std { using namespace experimental; }
#endif

#include <cstdio>

struct pt
{
    using handle_t = std::coroutine_handle<pt>;
    auto get_return_object() noexcept { return handle_t::from_promise(*this); }

    std::suspend_never initial_suspend() const noexcept { return {}; }
    std::suspend_never final_suspend() const noexcept { return {}; }
    void return_void() const noexcept {}
    void unhandled_exception() const noexcept {}
};

template <> struct std::coroutine_traits<pt::handle_t>
    { using promise_type = pt; };

static pt::handle_t foo()
{ 
    printf("from function\n");
    co_return;
}

auto bar = []() -> pt::handle_t
{ 
    printf("from lambda\n");
    co_return;
};

int main()
{
    foo();
    bar();
}
