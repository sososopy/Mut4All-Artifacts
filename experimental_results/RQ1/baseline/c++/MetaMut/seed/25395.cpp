
struct a {
	void inner() const {}
	void outer() const noexcept( noexcept( inner() ) ) {
		inner();
	}
};

