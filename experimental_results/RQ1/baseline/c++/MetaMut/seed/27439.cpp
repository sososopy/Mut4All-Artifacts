
#include <coroutine>

struct dummy
{
    struct promise_type
    {
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_never final_suspend() const noexcept { return {}; }
        dummy get_return_object() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() const noexcept {}
    };
};

struct awaitable {};

consteval auto operator co_await(awaitable) noexcept
{
    return std::suspend_always{};
}

dummy foo()
{ 
    co_await awaitable{};
}

int main() {}

