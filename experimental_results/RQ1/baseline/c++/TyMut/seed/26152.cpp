
struct A
{
  void f(){ A*&& a = this; }
};
int main(){}

