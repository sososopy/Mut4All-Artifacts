
module;

namespace n {

template<typename T>
void f() {
}

}

export module m;

export using n::f;
