
#include <iostream>
#include <vector>


class S {
public:
	S(const std::vector<char>& v_) : v{v_} {}
	void undefined() {
		std::cout << v.front() << std::endl;
	}
private:
	const std::vector<char>& v;
};

int main(){
	std::vector<char> foo {'f', 'a', 'i', 'l'};
	std::cout << foo.front() << std::endl;
	S s{foo};
	s.undefined();

	return 0;
}

