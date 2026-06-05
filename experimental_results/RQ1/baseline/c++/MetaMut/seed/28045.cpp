

#include <iostream>
#include <tuple>
#include <algorithm>

 void foo()
 {
     std::ostream& message_stream = std::cout;
     auto data = std::make_tuple(3,4.5,"cd");
     auto format = [&message_stream](auto && x) { message_stream << x ;};
    std::apply([&](auto const& ...x){(..., format(x));}, data);
 }

