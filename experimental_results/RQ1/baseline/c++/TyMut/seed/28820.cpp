
int main() {
  int &&i = 0;
  decltype(auto) j = i;
  return j;
}

