
void* po = 0;
void (*pf)() = reinterpret_cast<decltype(pf)>(po); // #2
static_assert(sizeof(po) >= sizeof(pf), "Conversion not supported");
