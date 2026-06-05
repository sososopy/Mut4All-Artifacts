
void f(void (*f1)(int)) {
  f1(42);
}

//Converts any lambda to a captureless lambda that converts nicely to a function pointer for the lifetime of the temporary.
template <typename Lambda>
static auto callback(Lambda &&l)
{
  static auto* p = &l;
  p = &l;
  return [](auto... x){ return (*p)(x...); };
}

int main() {
  int x = 5;
  f(callback([=](int y){std::cout << x << ' ' << y;}));
}

