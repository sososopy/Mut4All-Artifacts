
const unsigned ii = 0xdeadbeaf;
const int& dd = (const int&) ii;
int main(void) {
  return (ii == dd) ? 0: 1;
}
