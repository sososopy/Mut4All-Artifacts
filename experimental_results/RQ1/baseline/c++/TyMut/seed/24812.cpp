
#include <coroutine>
#include <vector>
#include <memory>
//#include <optional>

template<typename T>
struct Task {
    struct Awaitable {
        std::coroutine_handle<> coroutine;
        T result;
        
        bool await_ready() { return false; }
        void await_suspend(std::coroutine_handle<> h) { coroutine = h; }
        T await_resume() { return ::std::move(result); }
    };

    struct promise_type {
        T value;

        auto get_return_object() { return Task{ std::coroutine_handle<promise_type>::from_promise(*this) }; }
        auto initial_suspend() { return std::suspend_never{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void return_value(T val) { value = ::std::move(val); }
        void unhandled_exception() {}
    };

    std::coroutine_handle<promise_type> coroutine;

    Task(std::coroutine_handle<promise_type> coroutine) : coroutine(coroutine) {}

    ~Task() noexcept {
        if (coroutine)
            coroutine.destroy();
    }

    T getResult() {
        return coroutine.promise().value;
    }

    Awaitable operator co_await() {
        return { coroutine, ::std::move(coroutine.promise().value) };
    }
};

Task<::std::unique_ptr<int>> uniqueGuy() {
    co_return nullptr;
}

Task<::std::shared_ptr<int>> sharedGuy() {
    co_return nullptr;
}

//Task<::std::optional<int>> optionalGuy() {
//    co_return ::std::optional<int>();
//}

Task<int> intGuy()
{
    co_return 0;
}

Task<int> asyncFunction() {
//    // OK
//    ::std::vector iVec{ 
//        co_await intGuy(), 
//        co_await intGuy(), 
//    };
//
//    // OK
//    ::std::vector optVec{ 
//        co_await optionalGuy(), 
//        co_await optionalGuy(), 
//    };

//    // ICE
//    ::std::vector uniqueVec{ 
//        co_await uniqueGuy(), 
//        co_await uniqueGuy(), 
//    };
//
    // ICE
    ::std::vector<::std::unique_ptr<int>> uniqueVec2{ 
        co_await uniqueGuy(), 
        co_await uniqueGuy(), 
    };

//    // OK
//    ::std::vector<::std::unique_ptr<int>> uniqueVec3; 
//    uniqueVec3.push_back(co_await uniqueGuy());
//    uniqueVec3.push_back(co_await uniqueGuy());

//    // ICE
//    ::std::vector sharedVec{ 
//        co_await sharedGuy(), 
//        co_await sharedGuy(), 
//    };

    co_return 0;
}

int main() {
    return 0;
}

