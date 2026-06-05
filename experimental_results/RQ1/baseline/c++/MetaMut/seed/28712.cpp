
#include <coroutine>
#include <memory>

using std::suspend_never;

auto test_ptr(std::initializer_list<std::shared_ptr<int>> values) {
    return suspend_never{};
}
struct testing {
    struct promise_type {
        testing get_return_object() { return {}; }
        void return_void() {}
        suspend_never initial_suspend() noexcept { return {}; }
        suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() { std::terminate(); }
    };
};
testing demo() {
    co_await test_ptr({std::make_shared<int>(1)});
}

int main() {
    demo();
}
