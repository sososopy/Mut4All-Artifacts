
struct A {};

struct B
{
    B(int) {}
};

const A& f(const A& a, const B&) {
    return a;
}

const A& g(const A& a) {
    const A& result = f(a, 42);
    
    return result;
}
