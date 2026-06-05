
template <typename T>
struct numeric_limits;

template<>
struct numeric_limits<long>
{
    static constexpr long
		max() noexcept { return 0x7fffffffffffffffL; }
};

int main() {
	return 42ul > numeric_limits<long>::max();
}
