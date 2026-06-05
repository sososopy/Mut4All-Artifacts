
#include <ostream>
#include <vector>

struct CustomStream : std::ostream {};

std::ostream& operator<<(std::ostream& s, const std::vector<int>&) 
{
    return s;
}

CustomStream&& operator<<(CustomStream&& s, const std::vector<int>& v)  
{
    static_cast<std::ostream&>(s) << v;
    return std::move(s);
}

int main() 
{
    CustomStream() << std::vector<int>{};
}

