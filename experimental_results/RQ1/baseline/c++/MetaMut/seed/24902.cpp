
#include <coroutine>
#include <cstdlib>
#include <cstdint>
#include <iostream>

static constexpr std::size_t alignment = 1 << 13;

bool
is_aligned(void* p)
{
    return ((std::uintptr_t)p % alignment) == 0;
}

void* malloc_address = nullptr;
void* local_address = nullptr;

struct coro
{
    std::coroutine_handle<> coroutine_handle;

    struct promise_type
    {
        void*
        operator new(std::size_t n)
        {
            std::cout << "allocating " << n << " bytes\n";
            malloc_address = std::malloc(n);
            return malloc_address;
        }

        void
        operator delete(void* p)
        {
            std::free(p);
        }

        std::suspend_never
        initial_suspend()
        {
            return {};
        }

        coro
        get_return_object()
        {
            return {std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        void
        return_void()
        {}

        void
        unhandled_exception()
        {}

        std::suspend_always
        final_suspend() noexcept
        {
            return {};
        }
    };
};

struct S
{
    alignas(alignment) int x = 42;
};

coro
foo()
{
#if 0
    alignas(alignment) int x = 42;
#else
    S x;
#endif

    local_address = &x;
    std::cout << "is aligned? " << is_aligned(local_address) << std::endl;
    co_return;
}

int
main()
{
    std::cout << "max align: " << alignof(std::max_align_t) << std::endl;
    auto c = foo();
    std::cout << "malloc_address: " << malloc_address << std::endl;
    std::cout << "local_address: " << local_address << std::endl;
    std::cout << "difference: " << ((char*)local_address - (char*)malloc_address) << std::endl;
}
