
export module mod;

export template<typename BaseType>
struct downcast_base {
  using downcast_base_type = BaseType;
  friend auto downcast_guide(downcast_base);
};

export template<typename T> concept Downcastable = true;

export template<typename Target, Downcastable T>
struct downcast_child : T {
  friend auto downcast_guide(typename T::downcast_base) { return T(); }
};

export struct ratio { int num; };

export template<ratio R, typename U>
struct scaled_unit : downcast_base<scaled_unit<R, U>> {};

export template<typename Child>
struct unit : downcast_child<Child, scaled_unit<ratio(1), Child>> {};

export struct unknown_coherent_unit : unit<unknown_coherent_unit> {};

import mod;
struct u : downcast_base<u> { };
int main() { }
