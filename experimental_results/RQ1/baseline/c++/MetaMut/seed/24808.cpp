#include <array>
#include <cassert>
#include <coroutine>
#include <span>
#include <utility>

struct Generator {
    struct promise_type {
        int _val{};

        Generator get_return_object() noexcept
        {
            return {};
        }

        std::suspend_never initial_suspend() noexcept
        {
            return {};
        }

        std::suspend_always final_suspend() noexcept
        {
            return {};
        }

        std::suspend_always yield_value(int v) noexcept
        {
            _val = v;
            return {};
        }

        void return_void() noexcept {}
        void unhandled_exception() noexcept {}

        template<typename... TheRest>
        static void*
        operator new(size_t                    size,
                     std::span<unsigned char>& mem,
                     TheRest&&...)
        {
            assert(mem.size() > size);
            return mem.data();
        }

        static void operator delete(void*, size_t)
        {
        }
    };
};

Generator coro(std::span<unsigned char>)
{
#ifndef NO_LAMBDA
    auto lamb = [](std::span<unsigned char>) -> Generator {
        co_yield 42;
    };

    std::array<unsigned char, 250> arenaA{};
    auto x = lamb(arenaA);
#endif

    co_yield 9;
}

int main()
{
    std::array<unsigned char, 800> arena{};

    Generator g{coro(arena)};
}