
#include <iostream>
#include <map>

class vertex_descriptor {
public:
	vertex_descriptor() : p(nullptr) {}
	vertex_descriptor(void *pp) : p(pp) {}

	operator bool() const {
		std::cout << __func__ << std::endl;
		return p;
	}

	bool operator<(const vertex_descriptor b) const {
		std::cout << __func__ << std::endl;
		return p < b.p;
	}
private:
	void *p;
};

int main()
{
	std::map<std::pair<vertex_descriptor, int>, vertex_descriptor> vs;
	vertex_descriptor v1(nullptr), v2((void *)1);

	vs[std::make_pair(v1, 0)] = v1;
	vs[std::make_pair(v2, 0)] = v2;
	return 0;
}

