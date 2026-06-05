
#include <coroutine>

struct Generator
{
    struct promise_type
    {
        Generator get_return_object() { return Generator{}; }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int) { return {}; }
        void unhandled_exception() {}
    };
};

template <class F>
auto invoke_and_await(F) -> Generator
{
    co_yield 1;
}

auto test()
{
    invoke_and_await([] {});
}
