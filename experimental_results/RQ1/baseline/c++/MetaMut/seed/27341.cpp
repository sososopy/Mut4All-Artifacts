
#include <coroutine>

struct dummy
{
    struct promise_type
    {
        dummy get_return_object() const noexcept { return {}; }
        static dummy get_return_object_on_allocation_failure() noexcept { return {}; }

        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_never final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() const noexcept {}
    };
};

dummy foo()
{
    co_return;
}

int main() {}
