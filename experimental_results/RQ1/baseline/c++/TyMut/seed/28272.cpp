int bar{};

auto f = [b{bar}](){};  // fine
auto g = [&b=bar](){};  // fine
auto h = [&b{bar}](){}; // error

