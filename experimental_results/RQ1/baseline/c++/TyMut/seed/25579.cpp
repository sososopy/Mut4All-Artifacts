
void bar();
namespace foo { using ::bar; }
using foo::bar;
void bar() {}
