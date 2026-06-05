
#include <list>

class OptSpecifier {
  unsigned ID = 0;

public:
  explicit OptSpecifier(bool) = delete; // Works if this is commented
  OptSpecifier(unsigned ID) : ID(ID) {}
};

//void f(llvm::ArrayRef<llvm::opt::OptSpecifier>) {}
void f(std::list<OptSpecifier>) {}

int main()
{
    f({1U, 2U, 3U}); // Works
    f({1, 2, 3}); // Fails with gcc 13
}
