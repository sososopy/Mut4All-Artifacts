
#include <coroutine>

struct task {
    struct promise_type {
        task get_return_object();
        void return_void();
        void unhandled_exception();
        std::suspend_always initial_suspend() noexcept;
        std::suspend_always final_suspend() noexcept;
    };
};

struct wrapper {
    using promise_type = task::promise_type;
    wrapper(task&&);
};

wrapper f() {
    co_return;
}
