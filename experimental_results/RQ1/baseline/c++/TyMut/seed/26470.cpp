
#include <coroutine>

extern "C" int puts(const char* s);

struct task {
    struct promise_type {
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept {
            puts("final suspend");
            return {};
        }
        void return_void() { puts("returned"); }
        void unhandled_exception() {}
        task get_return_object() { return task{this}; }
    };

    promise_type* promise;
};

struct awaitable {
    bool await_ready() const noexcept { puts("ready"); return false; }
    void await_suspend(std::coroutine_handle<> h) {
        puts("suspend");
        h.resume();
    }
    bool await_resume() {
        puts("resume");
        return false;
    }
};

task example() {
    do {
        co_await awaitable{};
    } while (false && co_await awaitable{});
    co_return;
}

int main() {
    using handle = std::coroutine_handle<task::promise_type>;
    task t = example();
    handle h = handle::from_promise(*t.promise);
    h.resume();
}
