
struct A { //only happens if B is wrapped in another struct
    template<int=0> //only happens if B is a template
    struct B {
        struct C {
            int x = 0;
            double y = x; //this line must be present to produce the problem
        } c; //only happens if B contains a C object
    };
};
int main() {
    A::B<>();
}
