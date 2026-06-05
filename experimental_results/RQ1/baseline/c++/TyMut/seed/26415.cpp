
struct X {
  void * a = nullptr;
  void * b = nullptr;
};

struct alignas(16) AlignedData { };

struct A : virtual AlignedData {
  X xxx;
};

struct B : virtual AlignedData {};

struct Test : B, A {};

Test* t = new Test;

int main() {}
