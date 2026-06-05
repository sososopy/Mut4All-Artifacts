
//mod.cpp:

export module mod;

namespace mod {

export struct X {
  X() { }

  constexpr void f() {
    [](auto) { }(0);
  }
};

}

//test.cpp:

import mod;

int main() {
  mod::X{}.f();
}
