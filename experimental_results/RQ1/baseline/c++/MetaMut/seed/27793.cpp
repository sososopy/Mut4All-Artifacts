
#include <vector>

enum class value_type_t {
	array,
	null,
};

class Value {
	void clear() {
		switch(value_type) {
			case value_type_t::array:
				u.array_elements.~array_type();
				break;
			case value_type_t::null:
				break;
		}
		value_type = value_type_t::null;
	}
public:
	using array_type = std::vector<Value>;
	
	value_type_t value_type;
	union U {
		U() {}
		~U() {}
		array_type array_elements;
	} u;
	
	Value()
	  : value_type{value_type_t::null}
	{}
	
	Value(const Value &v)
	  : value_type{value_type_t::null}
	{
		*this = v;
	}
	
	//This is the constructor that unexpectedly calls itself
	Value(const array_type &a) {
		new(&u.array_elements) array_type{a};
		value_type = value_type_t::array;
	}
	
	~Value() {
		clear();
	}
	
	
	Value& operator=(const Value &v) {
		if(this!=&v) {
			clear();
			switch(v.value_type) {
				case value_type_t::array:
					new(&u.array_elements) array_type{v.u.array_elements};
					value_type = value_type_t::array;
					return *this;
				case value_type_t::null:
					value_type = value_type_t::null;
					break;
			}
		}
		return *this;
	}
	
};


int main(void) {
	Value v{Value::array_type{}};
}
