
template<typename T, typename U=void>
concept C = true;

void foo() {
  C decltype a19 = 1;
}

