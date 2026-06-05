
class X {

public:
  X() { std::cout << "Default" << std::endl; }
  X(int a) { std::cout << a << std::endl; }
};

class Y : public X { };

class Z : public Y {
  using X::X;
};

int main() {
  Z instance{3};  // how is Y constructed?
}

