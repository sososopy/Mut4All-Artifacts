
int* foo();

int main() {
  if (int* f = foo()) {
    return 1;
  }
  return 0;
}

