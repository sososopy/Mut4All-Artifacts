
class Wrapper {
public:
  static const char msgPtr[];
  static const char *m_fn1() { return &msgPtr[0]; }
};
const char Wrapper::msgPtr[];
int main() { Wrapper::m_fn1(); }
