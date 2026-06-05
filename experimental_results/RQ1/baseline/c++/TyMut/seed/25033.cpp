
template < int N >
constexpr int parse_int(char const(&arr)[N])noexcept{
	int number = 0;
	int base = 1;
	for(int i = 0; i < N; ++i){
		number += (static_cast< int >(arr[N - 1 - i]) - 48) * base;
		base *= 10;
	}
	return number;
}

// A compile time int-type
template < int I >
struct size{};

// Create a compile time int via user literals
template < char ... S >
constexpr auto operator"" _S()noexcept{
	return size< parse_int< sizeof...(S) >({S ...}) >();
}


// The problem:
template < typename T, int N >
void f(size< N >, T(&&)[N]){}


int main(){
	// OK: T is int
	f(2_S, {0, 1});

	// Bug: wrong type mismatch, T should be float
	f< float >(2_S, {0, 1});

	// OK: error conflicting types
	/* f(2_S, {0, 1.f}); */

	// Bug: wrong type mismatch, T should be float
	f< float >(2_S, {0, 1.f});
}
