
struct B {
  B(int, double) {}
};

void g(B) {};
int main(int argc, char** argv)
{
  g( {'a', 'b'} ); // OK: g(B(int,double)) user-defined conversion
  g( {1.0, 1.0} ); // error: narrowing
}
