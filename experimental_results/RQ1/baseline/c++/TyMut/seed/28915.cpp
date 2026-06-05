
//mod.cpp:

export module mod;
export struct B {
  virtual ~B() = default;
};


//main.cpp:

import mod;
int main() {
  struct D : B {};
  (void)D{};
}

