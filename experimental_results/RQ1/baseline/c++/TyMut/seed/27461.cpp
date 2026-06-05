
#ifndef __clang__
#include <coroutine>
#else
#include <experimental/coroutine>
namespace std { using namespace experimental; }
#endif

#include <cstdio>

struct rv
{
    void operator ()(){
        printf("call to operator\n");
    }
};

struct pt
{
    using handle_t = std::coroutine_handle<pt>;
    auto get_return_object() noexcept { return handle_t::from_promise(*this); }

    std::suspend_never initial_suspend() const noexcept { return {}; }
    std::suspend_never final_suspend() const noexcept { return {}; }
    //void return_void() const noexcept { printf("call to function\n"); }

    // error 1
    rv return_void;
    // error 2
    //static constexpr auto return_void = []{ printf("call to lambda\n");};
    void unhandled_exception() const noexcept {}
};

template <> struct std::coroutine_traits<pt::handle_t>
    { using promise_type = pt; };

static pt::handle_t foo()
{ 
    co_return;
}

int main()
{
    foo();
}
