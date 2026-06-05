
#include <coroutine>
#include <cstdio>

using namespace std;

struct generator {
    struct promise_type {
        generator get_return_object() {
            return generator{coroutine_handle<promise_type>::from_promise(*this)};
        }

        void return_void() {}
        void unhandled_exception() {}
        suspend_always initial_suspend() { return {}; }
        suspend_always final_suspend() { return {}; }
    };

    generator(coroutine_handle<promise_type> coro) {
        std::printf("generator() @ %p\n", this);
    }

    generator(generator&& g) noexcept {
        std::printf("generator(move from %p) @ %p\n", &g, this);
    }

    ~generator() {
        std::printf("~generator() @ %p\n", this);
    }
};

generator f() {
    co_return;
}

int main() {
    generator g = f();
}
