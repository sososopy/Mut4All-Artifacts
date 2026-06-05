
class C
{
  void f();
  int j = 10;
  int i =  [this]() { return this->j; }();
};

