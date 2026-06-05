
#include <string>

using std::string;

class Format {
 public:
  explicit Format(string formatted) {}
  string buffer;
};

string StrCat(const string& a) {
  return "";
}

template <typename... AV>
Format Message(string msg, const AV&... args) {
  return Format::Format(StrCat(msg, args...));
}

int main(int, char**) {
  Message("msg");
}

