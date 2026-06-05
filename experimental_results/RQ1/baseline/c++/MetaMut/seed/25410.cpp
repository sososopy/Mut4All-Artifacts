
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[])
{
  // The file must not terminates with the newline character.
  const char* file = "test.txt";
  std::ifstream in(file);
  if (!in) {
    std::cerr << "Could not work with file " << file << std::endl;
    return 1;
  }

  while (in) {
    std::string w;
    in >> w;
    std::cerr << "Fail: " << in.fail()
              << "   Good: " << in.good()
              << "   EOF: " << in.eof()
              << std::endl;
    // Assertion failed if the file doesn't ends with newline character.
    assert(!in.fail() == in.good());
  }

  return 0;
}


