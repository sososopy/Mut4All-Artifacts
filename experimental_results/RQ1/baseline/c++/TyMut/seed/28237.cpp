
#include <array>
#include <memory>

struct test_vector
{
    constexpr ~test_vector() noexcept
    {
        m_allocator.deallocate(m_begin, m_capacity);
    }

    constexpr test_vector(std::size_t n)
        : m_begin(m_allocator.allocate(n))
        , m_capacity(n)
    {
    }

    constexpr test_vector(test_vector const& other)
        : test_vector(other.m_capacity)
    {
    }

    constexpr test_vector() = delete;
    constexpr test_vector(test_vector&& other) noexcept = delete;
    constexpr auto operator=(test_vector const& other) -> test_vector& = delete;
    constexpr auto operator=(test_vector&& other) noexcept -> test_vector& = delete;

    std::allocator<int> m_allocator{};
    int*                m_begin;
    std::size_t         m_capacity;
};

static_assert( // This fails with "deallocation of already deallocated storage"
    []
    {
        std::array<test_vector, 1> const values{test_vector(42)};
        std::array<test_vector, 1> const copy = values;
        return true;
    }());
