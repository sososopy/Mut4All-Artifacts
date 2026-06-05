#include <coroutine>
#include <utility>
#include <cstddef>

struct dummy_coro {
	using promise_type = dummy_coro;
	bool await_ready() { return false; }
	void await_suspend(std::coroutine_handle<>) { }
	void await_resume() { }
	dummy_coro get_return_object() { return {}; }
	dummy_coro initial_suspend() { return {}; }
	dummy_coro final_suspend() { return {}; }
	void return_void() { }
	void unhandled_exception() { }
};

dummy_coro foo() {
	co_await []<size_t ...I>(std::index_sequence<I...>) -> dummy_coro {
		((co_await [](int){ return std::suspend_never{}; }(I)), ...);
	}(std::make_index_sequence<1>{});
}

