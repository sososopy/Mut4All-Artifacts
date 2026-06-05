
struct Destr
{ ~Destr(); };

struct XX
{
    Destr func() {
        throw "throw";
        return {};
    }
};

inline void call(Destr (XX::*f)(), XX *o)
{
    (o->*f)();
}

void f(XX *o) {
    call(&XX::func, o);
}
