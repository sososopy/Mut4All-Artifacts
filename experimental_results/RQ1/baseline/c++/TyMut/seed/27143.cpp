
template<unsigned TAG, typename T=const char*>
struct Tag {
	static constexpr auto tag = TAG;
	T value;
};

template<typename ... Tagged>
void bar(Tagged&& ... tagged) {
	const char* v[sizeof...(Tagged)] = {
        [Tagged::tag] = tagged.value ...
    }; 
}

using O = Tag<0>;

void foo() {
	bar(O{"ICE"});
}
