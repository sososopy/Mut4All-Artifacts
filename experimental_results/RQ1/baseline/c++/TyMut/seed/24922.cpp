

#include <string>
#include <unordered_map>

static const std::unordered_map<int, std::string> hashTable1 = {{2, "u"}};
static const std::unordered_map<std::string, std::string> hashTable2 = {{"u", "u"}};

const std::string& z1(int m);
const std::string& z1(int m) { return hashTable1.at(m); }
const std::string& z2(const std::string& m);
const std::string& z2(const std::string& m) { return hashTable2.at(m); }
const std::string& z3();
const std::string& z3() { return hashTable2.at("u"); }

int main() {
  const std::string& b1 { z1(2) };
  const std::string& b2 { z2("u") };
  const std::string& b3 { z3() };
  const std::string& f1 = hashTable1.at(2);
  const std::string& f2 = hashTable2.at("u");
  printf("%s %s %s %s %s\n", b1.c_str(), b2.c_str(), f1.c_str(), f2.c_str(), b3.c_str());
}

