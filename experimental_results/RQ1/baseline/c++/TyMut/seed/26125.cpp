
#include <iostream>
#include <cstddef>

struct A {};
typedef int A::*PointerToMember;

int main() {
   try {
      try {
         std::cout << "About to 'throw nullptr' (first case)" << std::endl;
         throw nullptr;
      } catch (PointerToMember) { // This *should* catch throw nullptr...
         std::cout << "Caught 'throw nullptr' as type 'int A::*PointerToMember'" << std::endl;
      }
   } catch (...) {
      std::cout << "Didn't catch 'throw nullptr' at all" << std::endl;
   }

   try {
      try {
         std::cout << "About to 'throw nullptr' (second case)" << std::endl;
         throw nullptr;
      } catch(void *) { // This *should* catch throw nullptr...
         std::cout << "Caught 'throw nullptr' as type 'void *'" << std::endl;
      }
   } catch (...) {
      std::cout << "Didn't catch 'throw nullptr' at all" << std::endl;
   }

   try {
      try {
         std::cout << "About to 'throw nullptr' (last case)" << std::endl;
         throw nullptr;
      } catch (PointerToMember) {
         std::cout << "Caught 'throw nullptr' as type 'int A::*PointerToMember'" << std::endl;
      } catch(void *) {
         std::cout << "Caught 'throw nullptr' as type 'void *'" << std::endl;
      } catch(std::nullptr_t) {
         std::cout << "Caught 'throw nullptr' as type 'std::nullptr_t'" << std::endl;
      }
   } catch (...) {
      std::cout << "Didn't catch 'throw nullptr' at all" << std::endl;
   }

   return 0;
}
