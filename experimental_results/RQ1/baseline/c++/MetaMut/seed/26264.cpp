

#include <iostream>
#include <vector>
#include <string>

struct VecPrinter {
  const std::vector<std::string>& v;
};

std::ostream& operator<<(std::ostream& out, VecPrinter vp)
{
  for (auto& i : vp.v)
    out << i << ' ';
  return out;
}

int main()
{
   std::vector<std::string> items {"default"};
    
   auto add = [=](std::string item) mutable 
              { items.push_back(item); return items; };

   std::cout << VecPrinter{ add("one") } << std::endl;
   std::cout << VecPrinter{ add("two") } << std::endl;
   std::cout << VecPrinter{ add("three") } << std::endl;
}

