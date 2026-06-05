
#include <iostream>

template<typename T>
void fun (T it) {
	int m = 0;
	for(int i = 0; i < 4; ++i, ++it){
		m |= int{char{*it}};
		std::cout << int(*it) << ", " << m << ", " << i << std::endl;
	}
}

int main () {
	char in[4] = {0,0,0,0};
	fun(in);
}
