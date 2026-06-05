
template<typename T>
struct Blah
{
  void operator()(int& a,int& b) {
    [this](auto&... args) {
      (operator()(args),...);
    }(a, b);
  }
};

int main() {
  Blah<void> proc;
  int a, b;
  proc(a, b);
}
