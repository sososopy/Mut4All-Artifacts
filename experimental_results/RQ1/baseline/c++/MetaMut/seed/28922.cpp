#include <coroutine>

struct dummy_coro {
	using promise_type = dummy_coro;
	bool await_ready() noexcept { return false; }
	void await_suspend(std::coroutine_handle<>) noexcept { }
	void await_resume() noexcept { }
	dummy_coro get_return_object() { return {}; }
	dummy_coro initial_suspend() { return {}; }
	dummy_coro final_suspend() noexcept { return {}; }
	void return_void() { }
	void unhandled_exception() { }
};

dummy_coro foo(int arg) {
	char arr[arg];
	co_return;
}

