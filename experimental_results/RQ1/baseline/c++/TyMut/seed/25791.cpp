
struct C {
  const static int asize = 3;
};

int main() {
  C c1;
  int table1[c1.asize] = { 0 }; // This compiles OK

  C *c2 = new C;
  int table2[c2->asize] = { 0 }; // This cause an error: variable-sized object may not be initialized
}
