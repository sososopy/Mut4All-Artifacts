
  #include <iostream>


  class Literal {
  private:
    Literal() = delete;
    Literal(const Literal&) = delete;
    Literal(Literal&&) = delete;
    Literal& operator=(const Literal&) = delete;
    Literal& operator=(Literal&&) = delete;
    ~Literal() = default;

  public:
    constexpr Literal(const int i);

  public:
    constexpr operator int() const;

  private:
    int int_;
  };

  constexpr Literal::Literal(const int i) : int_(i) {};

  constexpr Literal::operator int() const {
    return int_;
  }


  class Test {
  private:
    Test(const Test&) = delete;
    Test(Test&&) = delete;
    Test& operator=(const Test&) = delete;
    Test& operator=(Test&&) = delete;
    ~Test() = default;

  public:
    static constexpr const Literal kLiteral = Literal(99);

  public:
    constexpr Test();

  public:
    constexpr operator int() const;

  private:
    int int_;
  };

  constexpr Test::Test() : int_(kLiteral) {};

  constexpr Test::operator int() const {
    return int_;
  }


  int main() {
    std::cout << Literal(15) << std::endl;
    std::cout << Test() << std::endl;
    return 0;
  }
