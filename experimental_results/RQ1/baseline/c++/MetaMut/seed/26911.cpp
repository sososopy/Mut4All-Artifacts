
template <class>
class NumericArray {};

template <class>
constexpr bool match_numeric_array = false;
template <class Scalar>
constexpr bool
    match_numeric_array<NumericArray<Scalar>> =
        true;
template <class T>
concept bool cpt_NumericArrayContainer() {
  return match_numeric_array<T>;
}

template <class X>
concept bool cpt_NumericArray() {
  return requires{requires cpt_NumericArrayContainer<X>()};
}


template <class X>
requires !cpt_NumericArray<X>() auto func(int, X) {}

template <class X>
requires cpt_NumericArray<X>() auto func(int, X) {}

int main() {
  NumericArray<double> v5;
  func(0, v5);
}
