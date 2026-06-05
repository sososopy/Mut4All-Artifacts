
export module mod2;
namespace ns {
export int x = 0;
}


export module mod;
import mod2;
namespace ns {
export using ns::x;
}

import mod;
int main() { return ns::x; }
