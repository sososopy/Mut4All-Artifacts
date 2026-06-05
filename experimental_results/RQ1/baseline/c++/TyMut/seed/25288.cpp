
#include <functional>

using namespace std;

template<typename ActionType, typename... Cols>
void print_action(function<ActionType*(Cols..., ActionType)> action_factory)
{
}

int main(int argc, char *argv[])
{
  print_action<string, uint8_t>(function<string*(uint8_t, string)>());

  return 0;
}

