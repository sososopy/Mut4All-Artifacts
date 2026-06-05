struct X
{
private:
  ~X() {}
};

int main()
{
  new X;    // OK
  new X();  // OK
  new X{};  // ERROR
}
