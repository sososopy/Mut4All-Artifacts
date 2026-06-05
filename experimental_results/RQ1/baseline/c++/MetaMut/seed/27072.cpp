
#include <string>
struct T {
   std::string s[4065]; // 4064 is OK
};
int main() {
   T m = {"x", "y"};
} 


