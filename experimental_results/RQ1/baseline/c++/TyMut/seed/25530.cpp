
template<typename T> void use2(const T& arg) {
	arg.fun();
}

template<typename T> void use1(T value) {
	
	struct range_helper {
		T x;
		T fun() const { return x; }
	};
	use2(range_helper{value});
}

int main() {
	use1(3);
}
