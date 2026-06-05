
#if __has_include(<coroutine>)
#include <coroutine>
#else
#include <experimental/coroutine>
namespace std {
    using namespace std::experimental;
}
#endif
#include <cstdio>

struct X {};

int Y_live = 0;

struct Y
{
  Y () { std::puts("Y ()"); Y_live++; } 
  Y (const Y&) { std::puts("Y (const Y&)"); Y_live++; } 
  ~Y () { std::puts("~Y ()"); Y_live--; }
};

struct task {
    struct promise_type {
        void* operator new(size_t sz) {
            std::puts("operator new()");
            return ::operator new(sz);
        }

        void operator delete(void* p, size_t sz) {
            std::puts("operator delete");
            return ::operator delete(p, sz);
        }

        promise_type() {
            std::puts("promise_type()");
            // throw X{};
        }

        ~promise_type() {
            std::puts("~promise_type()");
        }

        struct awaiter {
            bool await_ready() {
                //throw X{};
                return false;
            }
            void await_suspend(std::coroutine_handle<>) {
                //throw X{};
            }
            void await_resume() {
                //throw X{};
            }
        };

        awaiter initial_suspend() {
            // throw X{};
            return {};
        }

        task get_return_object() {
            // throw X{};
            return task{};
        }

        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() noexcept {
            std::puts("unhandled_exception()");
        }
    };

    task() { std::puts("task()"); }
    ~task() { std::puts("~task()"); }
    task(task&&) { std::puts("task(task&&)"); }
};

task f(Y Val) {
    co_return;
}

int main() {
    Y Val;
    try {
        auto a = f(Val);
    } catch (X) {
        std::puts("caught X");
    }
}

