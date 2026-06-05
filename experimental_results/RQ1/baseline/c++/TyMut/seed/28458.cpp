
export module test;

export template <typename T>
struct test {
	T v_;

	test(T v) : v_(v) {}
	
	//inline
	~test() {}
};

export struct omg {
	using tt = test<bool>;

	static tt go(tt t) {
		return t;
	}
};
//end

//main.cpp
import test;

int main() {
	test<bool> t = false;
	return 0;
}
