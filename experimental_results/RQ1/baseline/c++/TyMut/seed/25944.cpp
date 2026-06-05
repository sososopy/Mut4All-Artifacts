
template< typename A, typename B >
struct bijection {
	auto size() const { return 0; } // error
//	auto size() const -> int { return 0; } // ok
};

int main() {
	bijection<int, int> m;
}

