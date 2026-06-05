
int main() {
	constexpr char ch = '=';

	[](auto) { return ch; }; // NOT OK
}
