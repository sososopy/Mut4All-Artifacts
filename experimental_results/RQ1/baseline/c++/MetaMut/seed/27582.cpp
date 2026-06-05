
#include <coroutine>

struct task
{
	struct promise_type
	{
		auto get_return_object() const { return task{}; }
		static constexpr std::suspend_never initial_suspend()  { return {}; }
		/*static*/ constexpr std::suspend_never final_suspend() { return {}; }
		/*static*/ constexpr void return_void() {}
		/*static*/ constexpr void unhandled_exception() {}
	};
};

task
test_task()
{
    co_await std::suspend_always{};
}

auto t = test_task();

int main() {}
