
int x = 0;
int z;

int main() {
  constexpr int& y = x;
  [=] { z = y; }();
}


