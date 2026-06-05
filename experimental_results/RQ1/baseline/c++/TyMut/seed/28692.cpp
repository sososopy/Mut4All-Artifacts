
#include <coroutine>

struct coro {
        struct promise_type {
                coro get_return_object() { return {}; }
                std::suspend_never initial_suspend() noexcept { return {}; }
                std::suspend_never final_suspend() noexcept { return {}; }
                void unhandled_exception() {}
                void return_void() {}
        };

        bool await_ready() { return true; }
        void await_resume() {}
        template <typename U>
        void await_suspend(U &) {}
};

struct b {
        ~b() {}
};

struct a {
        a(b) {}
        ~a() {}
};

coro f(b obj) {
        auto obj2 = a{obj};
        co_return;
}

int main() {
        f({});
}
