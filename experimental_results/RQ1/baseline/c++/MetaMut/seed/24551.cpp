
#include <cstdint>
#include <cstddef>


struct Foo {
	char data;
};


int main(int /*argc*/, char* /*argv*/[]) {
	int num = 6;
	#if 0 //No warning
		size_t offset = offsetof(struct Foo,data);
		delete [] new char[offset*static_cast<size_t>(num)];
	#else //warning
		delete [] new char[offsetof(struct Foo,data)*static_cast<size_t>(num)];
	#endif

	return 0;
}
