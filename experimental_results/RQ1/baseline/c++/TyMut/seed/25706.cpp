
#include <stdio.h>
#include <typeinfo>
struct __declspec(uuid("12345678-1234-1234-1234-123456789abC")) uuid;

int main() {
  &__uuidof(uuid);
  printf("%s\n", typeid(_GUID_12345678_1234_1234_1234_123456789abc).name());
}
