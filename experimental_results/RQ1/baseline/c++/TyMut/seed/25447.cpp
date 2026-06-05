

#include <vector>
#include <string>

class Expression
{
public:
  std::vector<int> args;
};


Expression getExpr(const Expression& def = {})
{
  return def;
}

int main()
{
  auto a = getExpr();
  

  // auto a = getExpr({});
}
