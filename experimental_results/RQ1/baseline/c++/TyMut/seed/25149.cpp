
#include <cstdint>
#include <cstdio>

enum ValueType : uint32_t {
  GOOD_VALUE = 0,
  BAD_VALUE
};

class Helper {
 public:
  Helper() {
    flag = true;
  }

  void run(uint32_t* value) {
    if (flag) {
      *value = GOOD_VALUE;
    }
  }

  bool flag;
};

class Runner {
 public:
  Runner() : value_type(BAD_VALUE) {
    Helper helper;
    helper.run(reinterpret_cast<uint32_t*>(&value_type));

    if (value_type == GOOD_VALUE) {
      fprintf(stderr, "Good!\n");  // Expected, works if non-optimized.
    } else {
      fprintf(stderr, "Bad!\n");  // Unexpected, happens for -O2 and -O3.
    }
  }

  // If we make this uint32_t, or make it local variable then bug disappears!
  ValueType value_type;
};

int main() {
  Runner runner;
  return 0;
}
