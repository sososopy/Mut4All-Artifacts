#include <coroutine>

struct dummy
{
    struct promise_type
    {
        dummy get_return_object() const noexcept { return {}; }
        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_never final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() const noexcept {}
    };
    // int i; // this kills the warning
};

dummy foo()
{ 
    co_return;
}

int main()
{
    foo();
}
