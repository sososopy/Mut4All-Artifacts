

long x;
auto& f(auto) { return x; }
auto* g(auto) { return &x; }
auto r = f(1);
auto p = g(1);

