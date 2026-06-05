
class C {
private:
    int a; int b;
public:
    C(int A, int B) : a(A), b(B) { }
    ~C() { }
};

struct y {
    int a; C b[];
} y = { 1, { { 2, 3 } } };

int main(void) { return 0; }

