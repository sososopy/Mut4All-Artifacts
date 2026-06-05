
#include <string>

std::string f(const std::string& s)
{
  return !s.empty() ? s : throw "Invalid";
}

int main(int argc, char** argv)
{
  return f(std::string(argc, 'x'));
}

