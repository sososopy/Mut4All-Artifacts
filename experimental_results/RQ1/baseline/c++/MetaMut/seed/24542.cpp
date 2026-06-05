
template <typename T>
struct Base {
  T value;
};

template <typename T>
struct Test : Base<T> {
  T test() {
    const int x = this->value;
    return ([&]{ return x; })();
  }
};

int main()  {
  Test<int> t;
  t.value = 0;
  return t.test();
}

