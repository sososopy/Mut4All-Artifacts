
#include <functional>
#include <map>
#include <string>
#include <tuple>

typedef std::tuple<std::string, std::string, double> Result;
typedef std::map<int, Result> Argument;
typedef std::function<const Result & (const Argument &)> Function;

std::function<Result ()> apply (const Argument &arg, const Function &fn)
{
  //  No trouble with any of these...
  //  return [&fn, &arg]() -> Result          { return fn(arg); };
  //  return [&fn, &arg]() -> const Result &  { return fn(arg); };
  //  return [&fn, &arg]()                    { Result r = fn(arg); return r; };

  //  But this causes heap corruption
      return [&fn, &arg]()                    { return fn(arg); };
}

const Result &func (const Argument &arg)
{
  //  std::map::at returns a const ref
  return arg.at(0);
}

int main (int argc, char *argv[])
{
  Argument arg;
  arg[0] = Result("", "a", 0);

  Function f = &func;
  auto g = apply(arg, f);
  g();
  return 0;
}

