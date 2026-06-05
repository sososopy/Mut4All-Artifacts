

namespace{

template<class, class>struct DoubleTrouble{};
struct ZeroTrouble{};
template <class T, class U>
struct Outer{
	using Trouble = DoubleTrouble<U,T>;

	template <class V>
	struct Inner{
		explicit Inner(const V& v, Trouble* trouble) {}
	};

	template <class V>
	Inner<V> factory(const V& v) {
		return Inner(v, &trouble_);
	}

 private:
	Trouble trouble_;
};

}

void boom(){
	Outer<char, int> ou;
	ZeroTrouble z;
	ou.factory(z);
}
