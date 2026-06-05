
#include <string>
enum profile_type {};
struct A {
  std::string value;
};
struct {
  profile_type type;
  A strategies[1];
} a{};


