
struct Z { };

struct A {
   operator Z &&() const = delete;  // GCC like this
   operator Z();
};

void zip() {
   Z &&x = A();
}
