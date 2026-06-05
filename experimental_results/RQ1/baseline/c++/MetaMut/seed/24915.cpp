

// m.cpp
module;

template <typename... _Elements> struct T;

template <typename H> struct T<H> {
  template <typename...> friend struct T;
};

export module M;
export template <typename=void> void fun() { T<int> t; }


// main.cpp
import M;

int main() { fun(); }
