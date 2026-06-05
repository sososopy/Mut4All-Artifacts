
#include <initializer_list>
void func( std::initializer_list<int[1]> list) {}

int main()
{
   func( {{1},{2},{3}} );
}

