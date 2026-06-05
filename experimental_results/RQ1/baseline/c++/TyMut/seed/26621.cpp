

struct A { static void mf() { } };

int main() {
   A a;
   void (&rmf)() = a.mf; // error
}
