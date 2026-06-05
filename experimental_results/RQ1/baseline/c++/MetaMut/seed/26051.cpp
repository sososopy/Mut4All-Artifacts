
#include <iostream>
#include <functional>

struct A {
      A(std::function< void (void)> lf) : m_f(lf) {};
      void exec() { m_f(); };

      std::function<void(void)> m_f;
      bool m_b = false;
};

namespace {
   A x([](){ x.m_b = true; std::cout << "x.m_b: " << x.m_b << std::endl; });
}

int main()
{
   x.exec();
   return 0;
}

