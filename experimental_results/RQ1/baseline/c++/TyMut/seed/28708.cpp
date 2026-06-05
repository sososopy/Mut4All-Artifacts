
#include <memory>
#include <new>

template<typename T> struct S : std::allocator<T>  {
	std::size_t sz;
	T *ps;
	
    template<std::size_t N> 
    constexpr S(typename std::add_const<T>::type (&p)[N]) :
			sz { N }, ps {this->allocate(N)} {
		for (std::size_t k = 0; k<N; ++k) {
			std::construct_at(ps+k, p[k]);
		}
	}
	constexpr ~S() {
		for (std::size_t k = 0; k < this->sz; ++k) {
			std::destroy_at(ps+k);
		}
		this->deallocate(this->ps, this->sz);
	}
};

constexpr S<char> str("Hello!");
