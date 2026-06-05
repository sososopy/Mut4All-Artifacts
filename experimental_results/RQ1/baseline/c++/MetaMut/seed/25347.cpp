

struct X{
  X(int, int) {}
};

struct Y {
 explicit Y(X) {}
 explicit Y(int, int) {}
};

int main() {
  Y({1, 2});
}


