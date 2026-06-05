#include <coroutine>
#include <memory>

struct my_params {
	std::unique_ptr<int> ptr; // or any other object with nontrivial dtor
};

static std::suspend_never take_params(my_params params)
{
	return {};
}

struct coro_t {
	struct promise_type {
		coro_t get_return_object() { return {}; }
		std::suspend_never initial_suspend() { return {}; }
		std::suspend_never final_suspend() noexcept { return {}; }
		void unhandled_exception() noexcept {}
	};
};

static coro_t my_coro()
{
	co_await take_params({ std::make_unique<int>(5) });
}

int main()
{
	my_coro();
}

