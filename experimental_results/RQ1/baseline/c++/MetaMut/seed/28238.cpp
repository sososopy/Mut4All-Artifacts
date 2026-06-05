
struct Mutt {
    operator int*() &&;
};

int* five(Mutt x) {
  return x;  // OK since C++20 because P1155
}

